/*
 * WSTAG210: Main Lobby, Asuka City.
 */

#include "common.h"
#define STAGE_TWEEN /* stageFuncs is a StageFuncs (stage.h) */
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern s32 listWindowFrames[];

/* The text file of the menus, which the versions number differently */
#if VERSION_US || VERSION_JP
#define MENU_TEXT TEXT_FILE(0x10C)
#elif VERSION_EU
#define MENU_TEXT 0x112
#endif

#include "common/list_menu.inc.c"

/* A list of up to eight options that each open a message */
void updateListMenu(StageListMenu *task, StageListMenuChildren *children) {
    SpriteDrawer drawer;
    s32 prev;

    switch (task->state) {
    case TASK_INIT:
    default:
        createListMenu(task, children);
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
        default:
            stageFuncs.start(&task->tweens[0], 1);
            task->substate++;
            break;
        case 1:
            if (stageFuncs.update(&task->tweens[0])) {
                showListMenuOptions(task, children, 0x1);
                task->substate++;
            }
            break;
        case 2:
            prev = task->cursor;
            moveListMenuCursor(task);
            if (prev != task->cursor) {
                SOUND.playSound(SOUND_CURSOR);
                children->cursor->setPos(children->cursor, 0xAF, task->cursor * 14 + 0x21);
                break;
            }
            if (PAD_PRESSED(PAD_CROSS)) {
                SOUND.playSound(SOUND_SELECT);
                if (task->cursor == task->count - 1) {
                    task->substate = 10;
                } else {
                    task->substate++;
                }
            } else if (PAD_PRESSED(PAD_TRIANGLE)) {
                SOUND.playSound(SOUND_MENU_CANCEL);
                task->substate = 10;
            }
            break;
        case 3:
            children->cursor->setStill(children->cursor, 1);
            children->cursor->setPalette(children->cursor, PALETTE_GREY);
            stageFuncs.start(&task->tweens[1], 1);
            task->substate++;
            break;
        case 4:
            if (stageFuncs.update(&task->tweens[1])) {
                openListMenuMessage(task, children, 0x1);
                task->substate++;
            }
            break;
        case 5:
            readListMenuMessage(task, children);
            break;
        case 6:
            stageFuncs.start(&task->tweens[1], 0);
            children->message->setVisible(children->message, 0);
            task->substate++;
            break;
        case 7:
            if (stageFuncs.update(&task->tweens[1])) {
                children->cursor->setStill(children->cursor, 0);
                children->cursor->setPalette(children->cursor, PALETTE_WHITE);
                task->substate = 2;
            }
            break;
        case 10:
            closeListMenu(task, children);
            break;
        case 11:
            if (stageFuncs.update(&task->tweens[0])) {
                task->state = TASK_KILL;
            }
            break;
        }
        initSpriteDrawer(&drawer);
        drawer.setLayerId(FIELD_LAYER_MAP, 2);
        drawer.setTexture(0x140, 0);
        drawer.setFollowScroll(0);
        if (task->showArrow) {
            if (GFX.funcs.getTime() - task->arrowTime >= 4) {
                task->arrowTime = GFX.funcs.getTime();
                if (++task->arrowFrame >= 5) {
                    task->arrowFrame = 0;
                }
            }
            drawer.setClutRow(task->arrowFrame);
            drawer.draw(FILE_CACHE.getEntry(MENU_SPRITES), 0xA, 0x124, 0xCD);
            drawer.setClutRow(0);
        }
        if (task->tweens[0].level != 0) {
            if (task->tweens[0].level != 0x1000) {
                drawer.setScale(task->tweens[0].level, 0x1000, 0x1000);
                drawer.setPivot(0x140, 0x56);
            }
            drawer.draw(FILE_CACHE.getEntry(MENU_SPRITES), listWindowFrames[task->count - 5], 0xA8, 0x18);
        }
        if (task->tweens[1].level != 0) {
            if (task->tweens[1].level != 0x1000) {
                drawer.setScale(task->tweens[1].level, 0x1000, 0x1000);
                drawer.setPivot(0x140, 0x56);
            }
            drawer.draw(FILE_CACHE.getEntry(MENU_SPRITES), 0x45, 0, 0xAC);
        }
        break;
    case TASK_DONE:
        stageFuncs.start(&task->tweens[0], 1);
        stageFuncs.update(&task->tweens[0]);
        break;
    case TASK_KILL:
        break;
    }
}

/*
 * Starts event 8, the guide to Digimon Online: a list of topics (Objective,
 * City Guide, Field Guide, Battle Tips, Training Basics, Status Screen...)
 * that each open a message
 */
void *startEvent8(void) {
    return createTask(updateListMenu, sizeof(StageListMenu), sizeof(StageListMenuChildren));
}

/* A two-option menu: creates the event object of the chosen option */
void updateEvent9(StageMenu *task, ChoiceChildren *children) {
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
                    children->options[j]->setString(children->options[j], FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, 0x2)), j + 2);
                }
                children->cursor->setVisible(children->cursor, 1);
                children->title->setString(children->title, FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, 0x2)), 1);
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
            children->event = FIELDSTG_startEvent(task->cursor == 0 ? 0xD : 0xE);
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
 * Starts event 9, which asks whether to download the partner Digimon now:
 * yes starts event 13, no event 14
 */
void *startEvent9(void) {
    return createTask(updateEvent9, sizeof(StageMenu), sizeof(ChoiceChildren));
}

/* A two-option menu: creates the event object of the chosen option */
void updateEvent1510(StageMenu *task, ChoiceChildren *children) {
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
                    children->options[j]->setString(children->options[j], FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, 0x35)), j + 2);
                }
                children->cursor->setVisible(children->cursor, 1);
                children->title->setString(children->title, FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, 0x35)), 1);
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
            children->event = FIELDSTG_startEvent(task->cursor == 0 ? 0x3A : 0x5E7);
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
 * Starts event 1510, which asks whether to listen to basic tamer
 * information: yes starts event 58, no event 1511
 */
void *startEvent1510(void) {
    return createTask(updateEvent1510, sizeof(StageMenu), sizeof(ChoiceChildren));
}

/* Creates the event object of the story so far, the first that applies */
void updateStage(StageTask *task, void **children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        do {
            if (GAME.progress == 0xD && FLAGS_00.checkCondition(FLAG(0x1C, 0xB), 0)) {
                children[0] = FIELDSTG_startEvent(0x136);
                break;
            }
            if (GAME.progress == 0x17 && FLAGS_00.checkCondition(FLAG(0x40, 0x50), 1)) {
                children[0] = FIELDSTG_startEvent(0x2AD);
                break;
            }
        } while (0);
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

/* After event 310, with Kail: sets flag 0x4005 and keeps it from playing again */
void endEvent310(void) {
    FLAGS_00.applyAction(FLAG(0x40, 5), 1);
    FLAGS_00.applyAction(FLAG(0x1C, 0xB), 1);
}

/*
 * After event 685, with Teddy: moves the story on to its step 0x18 and
 * clears flag 0x1C26 (which decides whether actors 17, 19, 21 and 23 in
 * WSTAG660 appear)
 */
void endEvent685(void) {
    FLAGS_00.applyAction(FLAG(0x1C, 0x26), 0);
    GAME.progress = 0x18;
}

/* the color the setup copies to FIELDSTG_state.spriteColor */
const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0 };

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xCD)
#define EVENT_TEXT_FILE 0x10B
#define STAGE_FILE 0x18D
#define STAGE_ARCHIVE 0x313
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xC5)
#define EVENT_TEXT_FILE 0x112
#define STAGE_FILE 0x19B
#define STAGE_ARCHIVE 0x322
#endif
/*
 * Sets the stage up: its map, actors, battles and events, playing KANRIBGM;
 * BGM_0029 instead at story steps 0x14 to 0x17 and 0x27 to 0x28
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x11C00, 0x13400};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 5;
    FIELDSTG_state.music = MUSIC(5, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
    FIELDSTG_state.battles = stageBattles;
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
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

#include "common/start_tween.inc.c"
#include "common/update_tween.inc.c"

s16 script13[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0x160, 0x110, 5),
    SCRIPT_PLACE(0x20, 0x180, 0xFF),
    SCRIPT_POSE(0x20, 1, 1),
    SCRIPT_PLACE(0x24, 0x1B2, 0xF7),
    SCRIPT_POSE(0x24, 1, 7),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x20, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_LEAVE(0x204, 0x160, 0x110, 5),
    SCRIPT_END,
};
s16 script14[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_WALK(2, 0x160, 0x110, 5),
    SCRIPT_PLACE(0x20, 0x180, 0xFF),
    SCRIPT_POSE(0x20, 1, 1),
    SCRIPT_PLACE(0x24, 0x1B2, 0xF7),
    SCRIPT_POSE(0x24, 1, 7),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x20, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x150, 0x118, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_END,
};
s16 script20[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0xB2, 0x167, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_EU
    0x5DE0,
#endif
};
s16 script30[] = {
    SCRIPT_FOLLOW(1, 2),
    SCRIPT_POSE(2, 1, 7),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x27F, 0x22D, 3),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_EU
    0x4A5F,
#endif
};
s16 script58[] = {
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x20, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x20, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON2, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x20, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script310[] = {
    SCRIPT_FOLLOW(1, 0x6A),
    SCRIPT_PLACE(0x20, 0x180, 0xFF),
    SCRIPT_POSE(0x20, 1, 1),
    SCRIPT_PLACE(0x24, 0x1B2, 0xF7),
    SCRIPT_POSE(0x24, 1, 7),
    SCRIPT_PLACE(0x2D, 0x29F, 0x178),
    SCRIPT_POSE(0x2D, 1, 7),
    SCRIPT_PLACE(0x31, 0x178, 0xD5),
    SCRIPT_POSE(0x31, 1, 7),
    SCRIPT_PLACE(0x36, 0x217, 0x133),
    SCRIPT_POSE(0x36, 1, 1),
    SCRIPT_PLACE(0x37, 0x148, 0x104),
    SCRIPT_POSE(0x37, 1, 6),
    SCRIPT_PLACE(0x41, 0x122, 0x159),
    SCRIPT_POSE(0x41, 1, 5),
    SCRIPT_PLACE(0x6A, 0x13D, 0x137),
    SCRIPT_POSE(0x6A, 1, 3),
    SCRIPT_PLACE(0x6B, 0x11D, 0x127),
    SCRIPT_POSE(0x6B, 1, 7),
    SCRIPT_WAIT(0x78),
    SCRIPT_TALK(0, 1, 0x6A, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x6B, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x6A, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x6B, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 0x6A, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x6B, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_FOLLOW(0, 0x6B),
    SCRIPT_POSE(0x6B, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0x6A, 1, 4),
    SCRIPT_WALK(0x6B, 0x11D, 0xD8, 4),
    SCRIPT_WAIT_WALK(0x6B),
    SCRIPT_WALK(0x6B, 0x14A, 0xC4, 5),
    SCRIPT_WAIT_WALK(0x6B),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HIDE_OBJECTS(2), 0x6B),
    SCRIPT_WAIT(0xC),
    SCRIPT_WALK(0x6B, 0x18A, 0xA4, 5),
    SCRIPT_WAIT_WALK(0x6B),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_SHOW_OBJECTS(2), 0x6B),
    SCRIPT_WAIT(0x1E),
    SCRIPT_FOLLOW(0, 0x6A),
    SCRIPT_PLACE(0x6B, 0, 0),
    SCRIPT_POSE(0x6B, 1, 0),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x6A, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script320[] = {
    SCRIPT_FOLLOW(1, 0x6A),
    SCRIPT_POSE(0x6A, 1, 1),
    SCRIPT_COMMAND(0x323, 0x325, 0x6A),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x6A),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x6A, 0xE8, 0x14C, 5),
    SCRIPT_WAIT_WALK(0x6A),
    SCRIPT_POSE(0x6A, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x6A, 3),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x6A, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script330[] = {
    SCRIPT_FOLLOW(1, 0x6A),
    SCRIPT_POSE(0x6A, 1, 7),
    SCRIPT_COMMAND(0x323, 0x325, 0x6A),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0x6A),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(0x6A, 0x268, 0x21C, 3),
    SCRIPT_WAIT_WALK(0x6A),
    SCRIPT_POSE(0x6A, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x6A, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(0x6A, 1, 3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script685[] = {
    SCRIPT_PLACE(2, 0x18A, 0xA4),
    SCRIPT_POSE(2, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HIDE_OBJECTS(2), 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x14A, 0xC4, 1),
    SCRIPT_PLACE(0xB3, 0x18A, 0xA4),
    SCRIPT_POSE(0xB3, 1, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x12A, 0xD4, 5),
    SCRIPT_WALK(0xB3, 0x14A, 0xC4, 1),
    SCRIPT_WAIT_WALK(0xB3),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_POSE(0xB3, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_SHOW_OBJECTS(2), 2),
    SCRIPT_WAIT(6),
    SCRIPT_WALK(2, 0x11A, 0xDC, 1),
    SCRIPT_WALK(0xB3, 0x13A, 0xCC, 1),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_POSE(0xB3, 1, 1),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 2, 1),
    SCRIPT_POSE(2, 7, 5),
    SCRIPT_WAIT_BOX,
    SCRIPT_WALK(2, 0x11A, 0xFC, 4),
    SCRIPT_WAIT(0xC),
    SCRIPT_TALK(0, 2, 0xB3, 2),
    SCRIPT_WALK(0xB3, 0x11A, 0xDC, 0),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 4),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 1),
    SCRIPT_POSE(2, 7, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x327, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0xB3, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 2, 1),
    SCRIPT_POSE(2, 7, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0xB3, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 2, 1),
    SCRIPT_POSE(2, 7, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 8, 0xB3, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 9, 2, 1),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x327, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x325, 2),
    SCRIPT_WAIT(0x3C),
    SCRIPT_POSE(2, 1, 4),
    SCRIPT_COMMAND(0x323, 0x326, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xA, 2, 1),
    SCRIPT_POSE(2, 7, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(0x323, 0x325, 0xB3),
    SCRIPT_WAIT(0x3C),
    SCRIPT_COMMAND(0x323, 0x326, 0xB3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(0xB3, 1, 0),
    SCRIPT_WAIT_WALK(0xB3),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xB, 0xB3, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xC, 2, 1),
    SCRIPT_POSE(2, 7, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xD, 0xB3, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xE, 2, 1),
    SCRIPT_POSE(2, 7, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0xF, 0xB3, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x10, 2, 1),
    SCRIPT_POSE(2, 7, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_POSE(2, 1, 4),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 0x11, 0xB3, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_POSE(2, 1, 0),
    SCRIPT_POSE(0xB3, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WALK(2, 0x11A, 0x130, 0),
    SCRIPT_WALK(0xB3, 0x14A, 0xC4, 5),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_WALK(2, 0x9A, 0x170, 1),
    SCRIPT_WALK(0xB3, 0x18A, 0xA4, 5),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HIDE_OBJECTS(2), 2),
    SCRIPT_WAIT(0xC),
    SCRIPT_LEAVE(0x200, 0x3E8, 0xEC, 1),
    SCRIPT_END,
};
s16 script1511[] = {
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 1, 0x20, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
/* MENU_SPRITES' frame of the list's window, by the number of options (5 to 8) */
s32 listWindowFrames[] = {
    28, 27, 25, 36,
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
    0,
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
AreaBattle area3Battle0 = { 188, 15, MUSIC(2, 0) };
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
    { 132, 0, 0, { &area0Battles, &area1Battles, &area2Battles, &area3Battles } },
};
ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x180, 0x100, 0x1B4, 0x100, 0x1D0, 0, 0x170, 0x1F8 },
    { 0x1C0, 0x100, 0x1E8, 0x16E, 0x2A0, 0x6E, 0x140, 0x1F7 },
    { 0x140, 0x100, 0x178, 0x139, 0xE0, 0x39, 0x150, 0x1F7 },
    { 0x180, 0x100, 0x1B6, 0x1B9, 0x1D8, 0xB9, 0x160, 0x1F7 },
    { 0x1C0, 0x100, 0x1E0, 0x16E, 0x280, 0x6E, 0x170, 0x1F7 },
    { 0x1C0, 0x100, 0x1C0, 0x16E, 0x200, 0x6E, 0x140, 0x1F6 },
    { 0x180, 0x100, 0x1A0, 0x1A0, 0x180, 0xA0, 0x150, 0x1F6 },
    { 0x180, 0x100, 0x1A0, 0x1C8, 0x180, 0xC8, 0x160, 0x1F6 },
    { 0x1C0, 0x100, 0x1D8, 0x16E, 0x260, 0x6E, 0x170, 0x1F6 },
    { 0x1C0, 0x100, 0x1C8, 0x16E, 0x220, 0x6E, 0x140, 0x1F5 },
    { 0x1C0, 0x100, 0x1C8, 0x196, 0x220, 0x96, 0x160, 0x1F5 },
    { 0x1C0, 0x100, 0x1E8, 0x1B6, 0x2A0, 0xB6, 0x170, 0x1F5 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x1C0, 0x100, 0x1D0, 0x196, 0x240, 0x96, 0x140, 0x1F4 },
    { 0x1C0, 0x100, 0x1D8, 0x196, 0x260, 0x96, 0x150, 0x1F4 },
    { 0x1C0, 0x100, 0x1E0, 0x196, 0x280, 0x96, 0x160, 0x1F4 },
    { 0x140, 0x100, 0x170, 0x139, 0xC0, 0x39, 0x170, 0x1F4 },
    { 0x1C0, 0x100, 0x1C0, 0x1B6, 0x200, 0xB6, 0x150, 0x1F3 },
    { 0x1C0, 0x100, 0x1C8, 0x1B6, 0x220, 0xB6, 0x160, 0x1F3 },
    { 0x1C0, 0x100, 0x1D0, 0x1B6, 0x240, 0xB6, 0x170, 0x1F3 },
    { 0x1C0, 0x100, 0x1D8, 0x1B6, 0x260, 0xB6, 0x140, 0x1F2 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x140, 0x100, 0x168, 0x139, 0xA0, 0x39, 0x150, 0x1F2 },
};
u16 actor0Talk0Conditions[] = { FLAG(0x1A, 0), 0, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0x1A, 0), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0x1A, 0), 1, CODES_END };
u16 actor0Talk1Actions[] = { START_EVENT(4), 1, CODES_END };
u16 actor1Talk0Actions[] = { START_EVENT(0x1A), 1, CODES_END };
u16 actor2Talk0Actions[] = { START_EVENT(0x1A), 1, CODES_END };
u16 actor3Talk0Actions[] = { START_EVENT(0x1A), 1, CODES_END };
u16 actor4Talk0Conditions[] = { FLAG(0x1A, 0), 0, CODES_END };
u16 actor4Talk0Actions[] = { FLAG(0x1A, 0), 1, CODES_END };
u16 actor4Talk1Conditions[] = { FLAG(0x1A, 0), 1, CODES_END };
u16 actor4Talk1Actions[] = { START_EVENT(3), 1, CODES_END };
u16 actor5Talk0Actions[] = { START_EVENT(3), 1, CODES_END };
u16 actor6Talk0Actions[] = { START_EVENT(3), 1, CODES_END };
u16 actor7Talk0Actions[] = { START_EVENT(3), 0, CODES_END };
u16 actor78Talk0Conditions[] = { FLAG(0x1A, 0x16), 0, CODES_END };
u16 actor78Talk0Actions[] = { FLAG(0x1A, 0x16), 1, CODES_END };
u16 actor78Talk1Conditions[] = { FLAG(0x1A, 0x16), 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 0x67 },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x486 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x67 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { NULL, actor2Talk0Actions, 0x429 },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { NULL, actor3Talk0Actions, 0x42B },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { actor4Talk0Conditions, actor4Talk0Actions, 0x66 },
    { actor4Talk1Conditions, actor4Talk1Actions, 0x34 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, actor5Talk0Actions, 0x34 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, actor6Talk0Actions, 0x35 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, actor7Talk0Actions, 0x37 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x11 },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x149 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x1A4 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x1A6 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x1A5 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x1A7 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x1A8 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x1A9 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { NULL, NULL, 0x1AE },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, NULL, 0x1AA },
    { NULL, NULL, 0 },
};
FieldTalk actor18Talks[] = {
    { NULL, NULL, 0x1AB },
    { NULL, NULL, 0 },
};
FieldTalk actor19Talks[] = {
    { NULL, NULL, 0x1AC },
    { NULL, NULL, 0 },
};
FieldTalk actor21Talks[] = {
    { NULL, NULL, 0xE },
    { NULL, NULL, 0 },
};
FieldTalk actor22Talks[] = {
    { NULL, NULL, 0x148 },
    { NULL, NULL, 0 },
};
FieldTalk actor23Talks[] = {
    { NULL, NULL, 0x199 },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0x10 },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0x147 },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, NULL, 0x18E },
    { NULL, NULL, 0 },
};
FieldTalk actor27Talks[] = {
    { NULL, NULL, 0xD },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { NULL, NULL, 0x14B },
    { NULL, NULL, 0 },
};
FieldTalk actor29Talks[] = {
    { NULL, NULL, 0x1BA },
    { NULL, NULL, 0 },
};
FieldTalk actor30Talks[] = {
    { NULL, NULL, 0x1BC },
    { NULL, NULL, 0 },
};
FieldTalk actor31Talks[] = {
    { NULL, NULL, 0x1BB },
    { NULL, NULL, 0 },
};
FieldTalk actor32Talks[] = {
    { NULL, NULL, 0x1BD },
    { NULL, NULL, 0 },
};
FieldTalk actor33Talks[] = {
    { NULL, NULL, 0x1BE },
    { NULL, NULL, 0 },
};
FieldTalk actor34Talks[] = {
    { NULL, NULL, 0x1BF },
    { NULL, NULL, 0 },
};
FieldTalk actor35Talks[] = {
    { NULL, NULL, 0x1C4 },
    { NULL, NULL, 0 },
};
FieldTalk actor36Talks[] = {
    { NULL, NULL, 0x1C0 },
    { NULL, NULL, 0 },
};
FieldTalk actor37Talks[] = {
    { NULL, NULL, 0x1C1 },
    { NULL, NULL, 0 },
};
FieldTalk actor38Talks[] = {
    { NULL, NULL, 0x1C2 },
    { NULL, NULL, 0 },
};
FieldTalk actor40Talks[] = {
    { NULL, NULL, 0xF0 },
    { NULL, NULL, 0 },
};
FieldTalk actor41Talks[] = {
    { NULL, NULL, 0x14C },
    { NULL, NULL, 0 },
};
FieldTalk actor42Talks[] = {
    { NULL, NULL, 0x1C5 },
    { NULL, NULL, 0 },
};
FieldTalk actor43Talks[] = {
    { NULL, NULL, 0xEF },
    { NULL, NULL, 0 },
};
FieldTalk actor44Talks[] = {
    { NULL, NULL, 0x14A },
    { NULL, NULL, 0 },
};
FieldTalk actor45Talks[] = {
    { NULL, NULL, 0x1AF },
    { NULL, NULL, 0 },
};
FieldTalk actor46Talks[] = {
    { NULL, NULL, 0x1D1 },
    { NULL, NULL, 0 },
};
FieldTalk actor47Talks[] = {
    { NULL, NULL, 0x1D3 },
    { NULL, NULL, 0 },
};
FieldTalk actor48Talks[] = {
    { NULL, NULL, 0x1D4 },
    { NULL, NULL, 0 },
};
FieldTalk actor49Talks[] = {
    { NULL, NULL, 0x1D2 },
    { NULL, NULL, 0 },
};
FieldTalk actor50Talks[] = {
    { NULL, NULL, 0x1D5 },
    { NULL, NULL, 0 },
};
FieldTalk actor51Talks[] = {
    { NULL, NULL, 0x1DA },
    { NULL, NULL, 0 },
};
FieldTalk actor52Talks[] = {
    { NULL, NULL, 0x1D6 },
    { NULL, NULL, 0 },
};
FieldTalk actor53Talks[] = {
    { NULL, NULL, 0x1D7 },
    { NULL, NULL, 0 },
};
FieldTalk actor54Talks[] = {
    { NULL, NULL, 0x1D8 },
    { NULL, NULL, 0 },
};
FieldTalk actor56Talks[] = {
    { NULL, NULL, 0x1DE },
    { NULL, NULL, 0 },
};
FieldTalk actor57Talks[] = {
    { NULL, NULL, 0x1DF },
    { NULL, NULL, 0 },
};
FieldTalk actor58Talks[] = {
    { NULL, NULL, 0x1DD },
    { NULL, NULL, 0 },
};
FieldTalk actor59Talks[] = {
    { NULL, NULL, 0x1DC },
    { NULL, NULL, 0 },
};
FieldTalk actor60Talks[] = {
    { NULL, NULL, 0x1E0 },
    { NULL, NULL, 0 },
};
FieldTalk actor61Talks[] = {
    { NULL, NULL, 0x1E5 },
    { NULL, NULL, 0 },
};
FieldTalk actor62Talks[] = {
    { NULL, NULL, 0x1E1 },
    { NULL, NULL, 0 },
};
FieldTalk actor63Talks[] = {
    { NULL, NULL, 0x1E2 },
    { NULL, NULL, 0 },
};
FieldTalk actor64Talks[] = {
    { NULL, NULL, 0x1E3 },
    { NULL, NULL, 0 },
};
FieldTalk actor72Talks[] = {
    { NULL, NULL, 0x42A },
    { NULL, NULL, 0 },
};
FieldTalk actor73Talks[] = {
    { NULL, NULL, 0x36 },
    { NULL, NULL, 0 },
};
FieldTalk actor74Talks[] = {
    { NULL, NULL, 0x1AD },
    { NULL, NULL, 0 },
};
FieldTalk actor75Talks[] = {
    { NULL, NULL, 0x1C3 },
    { NULL, NULL, 0 },
};
FieldTalk actor76Talks[] = {
    { NULL, NULL, 0x1D9 },
    { NULL, NULL, 0 },
};
FieldTalk actor77Talks[] = {
    { NULL, NULL, 0x1E4 },
    { NULL, NULL, 0 },
};
FieldTalk actor78Talks[] = {
    { actor78Talk0Conditions, actor78Talk0Actions, 0x4F },
    { actor78Talk1Conditions, NULL, 0x488 },
    { NULL, NULL, 0 },
};
FieldTalk actor80Talks[] = {
    { NULL, NULL, 0x2A4 },
    { NULL, NULL, 0 },
};
FieldTalk actor81Talks[] = {
    { NULL, NULL, 0x2A3 },
    { NULL, NULL, 0 },
};
FieldTalk actor85Talks[] = {
    { NULL, NULL, 0x38 },
    { NULL, NULL, 0 },
};
FieldTalk actor86Talks[] = {
    { NULL, NULL, 0x3A },
    { NULL, NULL, 0 },
};
FieldTalk actor87Talks[] = {
    { NULL, NULL, 0x3B },
    { NULL, NULL, 0 },
};
FieldTalk actor88Talks[] = {
    { NULL, NULL, 0x39 },
    { NULL, NULL, 0 },
};
FieldTalk actor89Talks[] = {
    { NULL, NULL, 0x3C },
    { NULL, NULL, 0 },
};
FieldTalk actor90Talks[] = {
    { NULL, NULL, 0x3D },
    { NULL, NULL, 0 },
};
FieldTalk actor91Talks[] = {
    { NULL, NULL, 0x41 },
    { NULL, NULL, 0 },
};
FieldTalk actor92Talks[] = {
    { NULL, NULL, 0x3E },
    { NULL, NULL, 0 },
};
FieldTalk actor93Talks[] = {
    { NULL, NULL, 0x3F },
    { NULL, NULL, 0 },
};
FieldTalk actor94Talks[] = {
    { NULL, NULL, 0x40 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor1Conditions[] = { SPECIAL(0x22), 1, PROGRESS(0x16), 0, CODES_END };
u16 actor2Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor5Conditions[] = { SPECIAL(0x22), 1, PROGRESS(0x16), 0, CODES_END };
u16 actor6Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor8Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor10Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor11Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor13Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor15Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor16Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor17Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor19Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor20Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor22Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor23Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor24Conditions[] = { PROGRESS(2), 1, ITEM(0, 0), 0, CODES_END };
u16 actor25Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor26Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor27Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor28Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor29Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor30Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor31Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor32Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor33Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor34Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor35Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor36Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor37Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor38Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor39Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor40Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor41Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor42Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor43Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor44Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor45Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor46Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor47Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor48Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor49Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor50Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor51Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor52Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor53Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor54Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor55Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor56Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor57Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor58Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor59Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor60Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor61Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor62Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor63Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor64Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor65Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor66Conditions[] = { PROGRESS(0xD), 1, FLAG(0x1C, 0xD), 0, CODES_END };
u16 actor67Conditions[] = { PROGRESS(0xD), 1, FLAG(0x1C, 0xD), 0, CODES_END };
u16 actor68Conditions[] = { SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor69Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor70Conditions[] = { SPECIAL(9), 1, SPECIAL(0x1A), 0, CODES_END };
u16 actor71Conditions[] = { PROGRESS(2), 1, CODES_END };
u16 actor72Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor73Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor74Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor75Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor76Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor77Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor78Conditions[] = { FLAG(0x1A, 0x15), 1, PROGRESS(0xC), 1, ITEM(0, 0xF), 0, CODES_END };
u16 actor79Conditions[] = { PROGRESS(0xD), 1, FLAG(0x1C, 0xD), 1, CODES_END };
u16 actor80Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor81Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor82Conditions[] = { PROGRESS(0x17), 1, CODES_END };
u16 actor83Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor84Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor85Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor86Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor87Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor88Conditions[] = { PROGRESS(0xD), 1, CODES_END };
u16 actor89Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor90Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor91Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor92Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor93Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor94Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor95Conditions[] = { PROGRESS(0x17), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x20, 4, 384, 255, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x20, 4, 384, 255, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x20, 4, 384, 255, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x20, 4, 384, 255, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x24, 5, 434, 247, 7 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x24, 5, 434, 247, 7 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x24, 5, 434, 247, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x24, 5, 434, 247, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x2D, 6, 684, 363, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x2D, 6, 684, 363, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x2D, 6, 684, 363, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x2D, 6, 684, 363, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x2D, 6, 529, 505, 5 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x2D, 6, 529, 505, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x2D, 6, 529, 505, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x2D, 6, 529, 505, 7 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x2D, 6, 305, 353, 7 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x2D, 6, 529, 505, 7 };
FieldActorEntry actor18 = { actor18Conditions, actor18Talks, 0x2D, 6, 529, 505, 7 };
FieldActorEntry actor19 = { actor19Conditions, actor19Talks, 0x2D, 6, 529, 505, 7 };
FieldActorEntry actor20 = { actor20Conditions, NULL, 0x2D, 6, 529, 505, 7 };
FieldActorEntry actor21 = { actor21Conditions, actor21Talks, 0x2E, 7, 391, 359, 1 };
FieldActorEntry actor22 = { actor22Conditions, actor22Talks, 0x2E, 7, 391, 359, 1 };
FieldActorEntry actor23 = { actor23Conditions, actor23Talks, 0x2E, 7, 391, 359, 1 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x2F, 8, 617, 501, 1 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0x2F, 8, 617, 501, 1 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x2F, 8, 617, 501, 1 };
FieldActorEntry actor27 = { actor27Conditions, actor27Talks, 0x31, 9, 361, 221, 7 };
FieldActorEntry actor28 = { actor28Conditions, actor28Talks, 0x31, 9, 361, 221, 7 };
FieldActorEntry actor29 = { actor29Conditions, actor29Talks, 0x31, 9, 361, 221, 7 };
FieldActorEntry actor30 = { actor30Conditions, actor30Talks, 0x31, 9, 361, 221, 7 };
FieldActorEntry actor31 = { actor31Conditions, actor31Talks, 0x31, 9, 193, 257, 7 };
FieldActorEntry actor32 = { actor32Conditions, actor32Talks, 0x31, 9, 193, 257, 7 };
FieldActorEntry actor33 = { actor33Conditions, actor33Talks, 0x31, 9, 193, 257, 7 };
FieldActorEntry actor34 = { actor34Conditions, actor34Talks, 0x31, 9, 193, 257, 7 };
FieldActorEntry actor35 = { actor35Conditions, actor35Talks, 0x31, 9, 391, 359, 7 };
FieldActorEntry actor36 = { actor36Conditions, actor36Talks, 0x31, 9, 193, 257, 7 };
FieldActorEntry actor37 = { actor37Conditions, actor37Talks, 0x31, 9, 193, 257, 7 };
FieldActorEntry actor38 = { actor38Conditions, actor38Talks, 0x31, 9, 193, 257, 7 };
FieldActorEntry actor39 = { actor39Conditions, NULL, 0x31, 9, 193, 257, 7 };
FieldActorEntry actor40 = { actor40Conditions, actor40Talks, 0x33, 0xA, 529, 505, 7 };
FieldActorEntry actor41 = { actor41Conditions, actor41Talks, 0x33, 0xA, 529, 505, 7 };
FieldActorEntry actor42 = { actor42Conditions, actor42Talks, 0x33, 0xA, 529, 505, 7 };
FieldActorEntry actor43 = { actor43Conditions, actor43Talks, 0x34, 0xB, 193, 257, 7 };
FieldActorEntry actor44 = { actor44Conditions, actor44Talks, 0x34, 0xB, 193, 257, 7 };
FieldActorEntry actor45 = { actor45Conditions, actor45Talks, 0x34, 0xB, 193, 257, 7 };
FieldActorEntry actor46 = { actor46Conditions, actor46Talks, 0x36, 0xC, 535, 307, 1 };
FieldActorEntry actor47 = { actor47Conditions, actor47Talks, 0x36, 0xC, 535, 307, 1 };
FieldActorEntry actor48 = { actor48Conditions, actor48Talks, 0x36, 0xC, 535, 307, 1 };
FieldActorEntry actor49 = { actor49Conditions, actor49Talks, 0x36, 0xC, 535, 307, 1 };
FieldActorEntry actor50 = { actor50Conditions, actor50Talks, 0x36, 0xC, 535, 307, 1 };
FieldActorEntry actor51 = { actor51Conditions, actor51Talks, 0x36, 0xC, 193, 257, 7 };
FieldActorEntry actor52 = { actor52Conditions, actor52Talks, 0x36, 0xC, 535, 307, 1 };
FieldActorEntry actor53 = { actor53Conditions, actor53Talks, 0x36, 0xC, 535, 307, 1 };
FieldActorEntry actor54 = { actor54Conditions, actor54Talks, 0x36, 0xC, 535, 307, 1 };
FieldActorEntry actor55 = { actor55Conditions, NULL, 0x36, 0xC, 535, 307, 1 };
FieldActorEntry actor56 = { actor56Conditions, actor56Talks, 0x37, 0xD, 328, 260, 6 };
FieldActorEntry actor57 = { actor57Conditions, actor57Talks, 0x37, 0xD, 328, 260, 6 };
FieldActorEntry actor58 = { actor58Conditions, actor58Talks, 0x37, 0xD, 328, 260, 6 };
FieldActorEntry actor59 = { actor59Conditions, actor59Talks, 0x37, 0xD, 328, 260, 6 };
FieldActorEntry actor60 = { actor60Conditions, actor60Talks, 0x37, 0xD, 328, 260, 6 };
FieldActorEntry actor61 = { actor61Conditions, actor61Talks, 0x37, 0xD, 684, 363, 3 };
FieldActorEntry actor62 = { actor62Conditions, actor62Talks, 0x37, 0xD, 328, 260, 6 };
FieldActorEntry actor63 = { actor63Conditions, actor63Talks, 0x37, 0xD, 328, 260, 6 };
FieldActorEntry actor64 = { actor64Conditions, actor64Talks, 0x37, 0xD, 328, 260, 6 };
FieldActorEntry actor65 = { actor65Conditions, NULL, 0x37, 0xD, 328, 260, 6 };
FieldActorEntry actor66 = { actor66Conditions, NULL, 0x6A, 0xE, 0, 0, 1 };
FieldActorEntry actor67 = { actor67Conditions, NULL, 0x6B, 0xF, 0, 0, 1 };
FieldActorEntry actor68 = { actor68Conditions, NULL, 0x70, 0x10, 360, 268, 1 };
FieldActorEntry actor69 = { actor69Conditions, NULL, 0x70, 0x10, 360, 268, 1 };
FieldActorEntry actor70 = { actor70Conditions, NULL, 0x71, 0x11, 450, 254, 7 };
FieldActorEntry actor71 = { actor71Conditions, NULL, 0x71, 0x11, 450, 254, 7 };
FieldActorEntry actor72 = { actor72Conditions, actor72Talks, 0x9D, 0x12, 384, 255, 1 };
FieldActorEntry actor73 = { actor73Conditions, actor73Talks, 0x9E, 0x13, 434, 247, 7 };
FieldActorEntry actor74 = { actor74Conditions, actor74Talks, 0x9F, 0x14, 529, 505, 7 };
FieldActorEntry actor75 = { actor75Conditions, actor75Talks, 0xA0, 0x15, 193, 257, 7 };
FieldActorEntry actor76 = { actor76Conditions, actor76Talks, 0xA2, 0x16, 535, 307, 1 };
FieldActorEntry actor77 = { actor77Conditions, actor77Talks, 0xAE, 0x17, 328, 260, 6 };
FieldActorEntry actor78 = { actor78Conditions, actor78Talks, 0xB2, 0x18, 285, 295, 5 };
FieldActorEntry actor79 = { actor79Conditions, NULL, 0xB2, 0x18, 0, 0, 1 };
FieldActorEntry actor80 = { actor80Conditions, actor80Talks, 0xB2, 0x18, 285, 295, 7 };
FieldActorEntry actor81 = { actor81Conditions, actor81Talks, 0xB3, 0x19, 308, 305, 3 };
FieldActorEntry actor82 = { actor82Conditions, NULL, 0xB3, 0x19, 0, 0, 1 };
FieldActorEntry actor83 = { actor83Conditions, NULL, 0x10E, 0x1A, 360, 268, 1 };
FieldActorEntry actor84 = { actor84Conditions, NULL, 0x10F, 0x1B, 450, 254, 7 };
FieldActorEntry actor85 = { actor85Conditions, actor85Talks, 0x175, 0x1C, 305, 353, 5 };
FieldActorEntry actor86 = { actor86Conditions, actor86Talks, 0x175, 0x1C, 305, 353, 5 };
FieldActorEntry actor87 = { actor87Conditions, actor87Talks, 0x175, 0x1C, 305, 353, 5 };
FieldActorEntry actor88 = { actor88Conditions, actor88Talks, 0x175, 0x1C, 305, 353, 5 };
FieldActorEntry actor89 = { actor89Conditions, actor89Talks, 0x175, 0x1C, 305, 353, 5 };
FieldActorEntry actor90 = { actor90Conditions, actor90Talks, 0x175, 0x1C, 305, 353, 5 };
FieldActorEntry actor91 = { actor91Conditions, actor91Talks, 0x175, 0x1C, 617, 501, 5 };
FieldActorEntry actor92 = { actor92Conditions, actor92Talks, 0x175, 0x1C, 305, 353, 5 };
FieldActorEntry actor93 = { actor93Conditions, actor93Talks, 0x175, 0x1C, 305, 353, 5 };
FieldActorEntry actor94 = { actor94Conditions, actor94Talks, 0x175, 0x1C, 305, 353, 5 };
FieldActorEntry actor95 = { actor95Conditions, NULL, 0x175, 0x1C, 305, 353, 5 };
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
    { 1, 0, 0x40, 2, 0x36, 2, 0, 1, 4, 0, 243, 69, 0, 0 },
    { 1, 0, 0x40, 2, 0x37, 2, 0, 1, 4, 0, 802, 278, 0, 0 },
    { 1, 0, 0x40, 2, 0x33, 2, 0, 1, 4, 0, 457, 129, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 0x16, 0, 339, 212, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 0x16, 0, 372, 196, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 0x16, 0, 403, 244, 0, 0 },
    { 1, 0, 0x40, 2, 0x32, 2, 0, 3, 0x16, 0, 435, 228, 0, 0 },
    { 1, 0, 0x40, 2, 0x55, 2, 0, 7, 0x12, 0, 386, 117, 0, 0 },
    { 1, 0, 0x40, 2, 0x55, 2, 0, 7, 0x12, 0, 546, 197, 0, 0 },
    { 1, 0, 0x40, 2, 0x55, 2, 0, 7, 0x12, 0, 674, 197, 0, 0 },
    { 1, 0, 0x40, 2, 0x56, 2, 0, 7, 0x12, 0, 573, 209, 0, 0 },
    { 1, 0, 0x40, 2, 0x56, 2, 0, 7, 0x12, 0, 613, 209, 0, 0 },
    { 1, 0, 0x40, 2, 0x57, 2, 0, 7, 0x12, 0, 151, 129, 0, 0 },
    { 1, 0, 0x40, 2, 0x57, 2, 0, 7, 0x12, 0, 411, 111, 0, 0 },
    { 1, 0, 0x40, 2, 0x57, 2, 0, 7, 0x12, 0, 649, 200, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 10, 105, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 42, 121, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 74, 137, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 290, 69, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 322, 85, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 354, 101, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 482, 165, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 514, 181, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 706, 213, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 738, 229, 0, 0 },
    { 1, 0, 0x40, 6, 0x55, 2, 0, 7, 0x12, 0, 770, 245, 0, 0 },
    { 1, 0, 0x40, 6, 0x57, 2, 0, 7, 0x12, 0, 119, 145, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 135, 232, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 155, 222, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 175, 212, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 195, 202, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 215, 192, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 235, 182, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 255, 172, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 275, 162, 0, 0 },
    { 1, 0, 0x40, 6, 0x58, 2, 0, 7, 0x12, 0, 295, 152, 0, 0 },
    { 1, 0x66, 0x40, 6, 4, 0, 0, 0, 0, 0, 335, 141, 0, 0 },
    { 1, 0x65, 0x40, 6, 5, 0, 0, 0, 0, 0, 511, 229, 0, 0 },
    { 1, 0x64, 0x40, 6, 6, 0, 0, 0, 0, 0, 736, 326, 0, 0 },
    { 1, 0, 0x40, 6, 0x34, 2, 0, 3, 4, 0, 114, 179, 0, 0 },
    { 1, 0, 0x40, 6, 0x35, 2, 0, 3, 4, 0, 264, 130, 0, 0 },
    { 1, 0, 0x40, 6, 0x59, 2, 0, 3, 4, 0, 429, 176, 0, 0 },
    { 1, 0, 0x40, 6, 0x51, 1, 0x51, 0x54, 8, 0, 585, 335, 0, 0 },
    { 1, 0, 0x40, 6, 0x4D, 1, 0x4D, 0x50, 8, 0, 585, 335, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x38, 1, 0x38, 0x3A, 0xA, 0, 28, 519, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x38, 1, 0x38, 0x3A, 0xA, 0, 81, 492, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x38, 1, 0x38, 0x3A, 0xA, 0, 183, 442, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x38, 1, 0x38, 0x3A, 0xA, 0, 237, 424, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x38, 1, 0x38, 0x3A, 0xA, 0, 656, 492, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x38, 1, 0x38, 0x3A, 0xA, 0, 752, 431, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 55, 506, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 100, 493, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 151, 461, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 202, 445, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 296, 424, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3B, 1, 0x3B, 0x3D, 0xA, 0, 657, 504, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 259, 428, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 366, 433, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 582, 411, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 661, 442, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 674, 537, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 694, 538, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x3E, 1, 0x3E, 0x40, 0xA, 0, 778, 417, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x41, 1, 0x41, 0x43, 0xA, 0, 449, 517, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x41, 1, 0x41, 0x43, 0xA, 0, 501, 544, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x41, 1, 0x41, 0x43, 0xA, 0, 530, 355, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x41, 1, 0x41, 0x43, 0xA, 0, 546, 354, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x41, 1, 0x41, 0x43, 0xA, 0, 636, 592, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x41, 1, 0x41, 0x43, 0xA, 0, 659, 600, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x44, 1, 0x44, 0x46, 0xA, 0, 87, 303, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x44, 1, 0x44, 0x46, 0xA, 0, 130, 322, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x44, 1, 0x44, 0x46, 0xA, 0, 682, 614, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x49, 0xA, 0, 61, 292, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x49, 0xA, 0, 113, 328, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x49, 0xA, 0, 474, 531, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x47, 1, 0x47, 0x49, 0xA, 0, 529, 554, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 437, 427, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 455, 421, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 608, 405, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 626, 396, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 654, 525, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 691, 424, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 144, 110, 160, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 359, 148, 193, 0 },
    { 1, 0, 0x40, 4, 2, 0, 0, 0, 0, 0, 535, 236, 281, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 759, 332, 376, 0 },
    { 1, 0, 0x50, 4, 7, 0, 0, 0, 0, 0, 441, 345, 450, 0 },
    { 1, 0, 0x40, 4, 8, 0, 0, 0, 0, 0, 464, 444, 496, 0 },
    { 1, 0, 0x40, 4, 9, 0, 0, 0, 0, 0, 595, 442, 478, 0 },
    { 1, 0, 0x40, 4, 0xA, 0, 0, 0, 0, 0, 384, 259, 279, 0 },
    { 1, 0, 0x40, 4, 0xB, 0, 0, 0, 0, 0, 401, 250, 271, 0 },
    { 1, 0, 0x40, 4, 0xC, 0, 0, 0, 0, 0, 368, 251, 270, 0 },
    { 1, 0, 0x40, 4, 0xD, 0, 0, 0, 0, 0, 417, 243, 264, 0 },
    { 1, 0, 0x40, 4, 0xE, 0, 0, 0, 0, 0, 352, 242, 262, 0 },
    { 1, 0, 0x40, 4, 0xF, 0, 0, 0, 0, 0, 433, 233, 255, 0 },
    { 1, 0, 0x40, 4, 0x10, 0, 0, 0, 0, 0, 336, 229, 255, 0 },
    { 1, 0, 0x40, 4, 0x11, 0, 0, 0, 0, 0, 449, 227, 247, 0 },
    { 1, 0, 0x40, 4, 0x12, 0, 0, 0, 0, 0, 320, 227, 247, 0 },
    { 1, 0, 0x40, 4, 0x13, 0, 0, 0, 0, 0, 337, 219, 240, 0 },
    { 1, 0, 0x40, 4, 0x14, 0, 0, 0, 0, 0, 353, 208, 231, 0 },
    { 1, 0, 0x40, 4, 0x15, 0, 0, 0, 0, 0, 369, 203, 223, 0 },
    { 1, 0, 0x40, 4, 0x16, 0, 0, 0, 0, 0, 385, 192, 215, 0 },
    { 1, 0, 0x40, 4, 0x17, 0, 0, 0, 0, 0, 401, 187, 207, 0 },
    { 1, 0, 0x40, 4, 0x18, 0, 0, 0, 0, 0, 499, 317, 334, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x200, 0x3E8, 0xEC, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x206, 0x70, 0xF0, 7, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x207, 0x58, 0x1CC, 5, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x208, 0x69, 0x134, 5, 0x65, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x214, 0x1F8, 0x2BC, 5, 0x66, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x217, 0x1D8, 0x1D4, 3, 0, 0, 0 },
    { { { PROGRESS(2), 1 }, { CODES_END, 0 } }, SLOT_EVENT, 0x14, 0, 0, 0, 0, 0, 0 },
    { { { PROGRESS(2), 1 }, { CODES_END, 0 } }, SLOT_EVENT, 0x1E, 0, 0, 0, 0, 0, 0 },
    { { { PROGRESS(0xD), 1 }, { CODES_END, 0 } }, SLOT_EVENT, 0x140, 0, 0, 0, 0, 0, 0 },
    { { { PROGRESS(0xD), 1 }, { CODES_END, 0 } }, SLOT_EVENT, 0x14A, 0, 0, 0, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageFuncs stageFuncs = { setupStage, startTween, updateTween };
FieldEvent stageEvents[] = {
    { 8, NULL, EVENT_TEXT(1), startEvent8, NULL },
    { 9, NULL, EVENT_TEXT(2), startEvent9, NULL },
    { 13, script13, EVENT_TEXT(6), NULL, NULL },
    { 14, script14, EVENT_TEXT(7), NULL, NULL },
    { 20, script20, EVENT_TEXT(8), NULL, NULL },
    { 30, script30, EVENT_TEXT(9), NULL, NULL },
    { 58, script58, EVENT_TEXT(0x12), NULL, NULL },
    { 310, script310, EVENT_TEXT(0x1C), NULL, endEvent310 },
    { 320, script320, EVENT_TEXT(0x1D), NULL, NULL },
    { 330, script330, EVENT_TEXT(0x1E), NULL, NULL },
    { 685, script685, EVENT_TEXT(0x22), NULL, endEvent685 },
    { 1510, NULL, EVENT_TEXT(0x35), startEvent1510, NULL },
    { 1511, script1511, EVENT_TEXT(0x30), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
