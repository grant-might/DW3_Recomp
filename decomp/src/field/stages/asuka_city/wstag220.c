/*
 * WSTAG220: Digimon Lab, Asuka City.
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
void updateEvent1512(StageMenu *task, ChoiceChildren *children) {
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
                    children->options[j]->setString(children->options[j], FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, 0x36)), j + 2);
                }
                children->cursor->setVisible(children->cursor, 1);
                children->title->setString(children->title, FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, 0x36)), 1);
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
            children->event = FIELDSTG_startEvent(task->cursor == 0 ? 0x35 : 0x5E9);
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
 * Starts event 1512, which asks whether to listen to partner Digimon
 * information: yes starts event 53, no event 1513
 */
void *startEvent1512(void) {
    return createTask(updateEvent1512, sizeof(StageMenu), sizeof(ChoiceChildren));
}

/* A two-option menu: creates the event object of the chosen option */
void updateEvent1514(StageMenu *task, ChoiceChildren *children) {
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
                    children->options[j]->setString(children->options[j], FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, 0x37)), j + 2);
                }
                children->cursor->setVisible(children->cursor, 1);
                children->title->setString(children->title, FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, 0x37)), 1);
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
            children->event = FIELDSTG_startEvent(task->cursor == 0 ? 0x37 : 0x5EB);
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
 * Starts event 1514, which asks whether to listen to digivolve information:
 * yes starts event 55, no event 1515
 */
void *startEvent1514(void) {
    return createTask(updateEvent1514, sizeof(StageMenu), sizeof(ChoiceChildren));
}

/* A two-option menu: creates the event object of the chosen option */
void updateEvent1516(StageMenu *task, ChoiceChildren *children) {
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
                    children->options[j]->setString(children->options[j], FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, 0x38)), j + 2);
                }
                children->cursor->setVisible(children->cursor, 1);
                children->title->setString(children->title, FILE_CACHE.getEntry(TEXT_ENTRY(MENU_TEXT, 0x38)), 1);
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
            children->event = FIELDSTG_startEvent(task->cursor == 0 ? 0x38 : 0x5ED);
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
 * Starts event 1516, which asks whether to listen to digivolve information:
 * yes starts event 56, no event 1517
 */
void *startEvent1516(void) {
    return createTask(updateEvent1516, sizeof(StageMenu), sizeof(ChoiceChildren));
}

#include "common/update_stage.inc.c"

#include "common/start_stage.inc.c"

/* the color the setup copies to FIELDSTG_state.spriteColor */
const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0 };

#if VERSION_US || VERSION_JP
#define STAGE_TEXT TEXT_FILE(0xF0)
#define EVENT_TEXT_FILE 0x10B
#define STAGE_FILE 0x18F
#define STAGE_ARCHIVE 0x2C7
#elif VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xE8)
#define EVENT_TEXT_FILE 0x112
#define STAGE_FILE 0x19D
#define STAGE_ARCHIVE 0x2D6
#endif
/*
 * Sets the stage up: its map, actors and events, playing the ambience
 * ENV_0011; BGM_0029 instead at story steps 0x14 to 0x17 and 0x27 to 0x28
 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE - 1;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16 | 1;
    FIELDSTG_state.objects = stageObjects;
    FIELDSTG_state.slots = stageSlots;
    FIELDSTG_state.imageFile = STAGE_ARCHIVE;
    FIELDSTG_state.start = (Vec2){0x10200, 0x15700};
    FIELDSTG_state.images.actors = stageImages;
    FIELDSTG_state.soundBank = 0x33;
    FIELDSTG_state.music = MUSIC(0x33, 0);
    FIELDSTG_state.actors = stageActors;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.spriteColor = stageColor;
    FIELDSTG_state.events = stageEvents;
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

#include "common/start_tween.inc.c"
#include "common/update_tween.inc.c"

s16 script53[] = {
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x24, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 4, 0x20, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 5, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON2, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 7, 0x24, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
    /* after the end, the original's padding up to a word, which isn't zeros */
#if VERSION_US
    0x3E0,
#elif VERSION_JP
    0x8FB0,
#endif
};
s16 script54[] = {
    SCRIPT_WALK(2, 0xBA, 0xF8, 5),
    SCRIPT_PLACE(0x20, 0xDC, 0xE8),
    SCRIPT_POSE(0x20, 1, 1),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_HALT_PARTNERS, 2),
    SCRIPT_WAIT_WALK(2),
    SCRIPT_POSE(2, 1, 5),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 1, 0x20, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON1, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 2, 0x32D, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON2, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 3, 0x20, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script55[] = {
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
s16 script56[] = {
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
    SCRIPT_TALK(0, 5, 2, 4),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_COMMAND(FIELD_TASK_COMMANDS, FIELD_COMMAND_ICON2, 2),
    SCRIPT_WAIT(0x1E),
    SCRIPT_WAIT(0x1E),
    SCRIPT_TALK(0, 6, 0x20, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1513[] = {
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 1, 0x24, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1515[] = {
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 1, 0x20, 2),
    SCRIPT_WAIT_BOX,
    SCRIPT_WAIT(0x1E),
    SCRIPT_END,
};
s16 script1517[] = {
    SCRIPT_WAIT(0x3C),
    SCRIPT_TALK(0, 1, 0x20, 2),
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
    { 0x140, 0x100, 0x16E, 0x178, 0xB8, 0x78, 0x150, 0x1F7 },
    { 0x140, 0x100, 0x176, 0x178, 0xD8, 0x78, 0x160, 0x1F7 },
    { 0x140, 0x100, 0x140, 0x17E, 0, 0x7E, 0x170, 0x1F7 },
    { 0x140, 0x100, 0x160, 0x180, 0x80, 0x80, 0x150, 0x1F6 },
    { 0x140, 0x100, 0x148, 0x17E, 0x20, 0x7E, 0x160, 0x1F6 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x140, 0x100, 0x150, 0x19E, 0x40, 0x9E, 0x170, 0x1F6 },
    { 0x140, 0x100, 0x158, 0x1A0, 0x60, 0xA0, 0x140, 0x1F5 },
    { 0x140, 0x100, 0x166, 0x1A0, 0x98, 0xA0, 0x150, 0x1F5 },
    { 0x140, 0x100, 0x16E, 0x1A0, 0xB8, 0xA0, 0x160, 0x1F5 },
    { 0x140, 0x100, 0x176, 0x1A0, 0xD8, 0xA0, 0x170, 0x1F5 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
};
u16 actor0Talk0Conditions[] = { FLAG(0x1A, 6), 0, CODES_END };
u16 actor0Talk0Actions[] = { FLAG(0x1A, 6), 1, CODES_END };
u16 actor0Talk1Conditions[] = { FLAG(0x1A, 6), 1, SPECIAL(0xD), 0, CODES_END };
u16 actor0Talk1Actions[] = { START_EVENT(7), 1, CODES_END };
u16 actor0Talk2Conditions[] = { FLAG(0x1A, 6), 1, SPECIAL(0xD), 1, CODES_END };
u16 actor0Talk2Actions[] = { START_EVENT(8), 1, CODES_END };
u16 actor1Talk0Conditions[] = { SPECIAL(0xD), 0, CODES_END };
u16 actor1Talk0Actions[] = { START_EVENT(7), 1, CODES_END };
u16 actor1Talk1Conditions[] = { SPECIAL(0xD), 1, CODES_END };
u16 actor1Talk1Actions[] = { START_EVENT(8), 1, CODES_END };
u16 actor2Talk0Conditions[] = { SPECIAL(0xD), 0, CODES_END };
u16 actor2Talk0Actions[] = { START_EVENT(7), 1, CODES_END };
u16 actor2Talk1Conditions[] = { SPECIAL(0xD), 1, CODES_END };
u16 actor2Talk1Actions[] = { START_EVENT(8), 1, CODES_END };
u16 actor3Talk0Conditions[] = { FLAG(0x1A, 7), 0, CODES_END };
u16 actor3Talk0Actions[] = { FLAG(0x1A, 7), 1, CODES_END };
u16 actor3Talk1Conditions[] = { FLAG(0x1A, 7), 1, CODES_END };
u16 actor3Talk1Actions[] = { START_EVENT(5), 1, CODES_END };
u16 actor4Talk0Actions[] = { START_EVENT(5), 1, CODES_END };
u16 actor5Talk0Conditions[] = { SPECIAL(0xD), 0, CODES_END };
u16 actor5Talk0Actions[] = { START_EVENT(5), 1, CODES_END };
u16 actor16Talk0Conditions[] = { FLAG(0x1A, 4), 0, CODES_END };
u16 actor16Talk0Actions[] = { FLAG(0x1A, 4), 1, CODES_END };
u16 actor16Talk1Conditions[] = { FLAG(0x1A, 4), 1, CODES_END };
u16 actor16Talk1Actions[] = { 0x7C00, 1, CODES_END };
u16 actor17Talk0Actions[] = { 0x7C00, 1, CODES_END };
u16 actor26Talk0Actions[] = { 0x7C00, 1, CODES_END };
FieldTalk actor0Talks[] = {
    { actor0Talk0Conditions, actor0Talk0Actions, 2 },
    { actor0Talk1Conditions, actor0Talk1Actions, 0x41F },
    { actor0Talk2Conditions, actor0Talk2Actions, 0x41F },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { actor1Talk0Conditions, actor1Talk0Actions, 0x424 },
    { actor1Talk1Conditions, actor1Talk1Actions, 0x424 },
    { NULL, NULL, 0 },
};
FieldTalk actor2Talks[] = {
    { actor2Talk0Conditions, actor2Talk0Actions, 0x41F },
    { actor2Talk1Conditions, actor2Talk1Actions, 0x41F },
    { NULL, NULL, 0 },
};
FieldTalk actor3Talks[] = {
    { actor3Talk0Conditions, actor3Talk0Actions, 3 },
    { actor3Talk1Conditions, actor3Talk1Actions, 0x420 },
    { NULL, NULL, 0 },
};
FieldTalk actor4Talks[] = {
    { NULL, actor4Talk0Actions, 0x425 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { actor5Talk0Conditions, actor5Talk0Actions, 0x420 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 1 },
    { NULL, NULL, 0 },
};
FieldTalk actor7Talks[] = {
    { NULL, NULL, 0x198 },
    { NULL, NULL, 0 },
};
FieldTalk actor8Talks[] = {
    { NULL, NULL, 0x18F },
    { NULL, NULL, 0 },
};
FieldTalk actor9Talks[] = {
    { NULL, NULL, 0x190 },
    { NULL, NULL, 0 },
};
FieldTalk actor10Talks[] = {
    { NULL, NULL, 0x191 },
    { NULL, NULL, 0 },
};
FieldTalk actor11Talks[] = {
    { NULL, NULL, 0x192 },
    { NULL, NULL, 0 },
};
FieldTalk actor12Talks[] = {
    { NULL, NULL, 0x193 },
    { NULL, NULL, 0 },
};
FieldTalk actor13Talks[] = {
    { NULL, NULL, 0x194 },
    { NULL, NULL, 0 },
};
FieldTalk actor14Talks[] = {
    { NULL, NULL, 0x195 },
    { NULL, NULL, 0 },
};
FieldTalk actor15Talks[] = {
    { NULL, NULL, 0x196 },
    { NULL, NULL, 0 },
};
FieldTalk actor16Talks[] = {
    { actor16Talk0Conditions, actor16Talk0Actions, 4 },
    { actor16Talk1Conditions, actor16Talk1Actions, 0xF1 },
    { NULL, NULL, 0 },
};
FieldTalk actor17Talks[] = {
    { NULL, actor17Talk0Actions, 0x426 },
    { NULL, NULL, 0 },
};
FieldTalk actor24Talks[] = {
    { NULL, NULL, 0x421 },
    { NULL, NULL, 0 },
};
FieldTalk actor25Talks[] = {
    { NULL, NULL, 0x422 },
    { NULL, NULL, 0 },
};
FieldTalk actor26Talks[] = {
    { NULL, actor26Talk0Actions, 0x423 },
    { NULL, NULL, 0 },
};
FieldTalk actor28Talks[] = {
    { NULL, NULL, 0x197 },
    { NULL, NULL, 0 },
};
u16 actor0Conditions[] = { SPECIAL(0x22), 1, PROGRESS(0x16), 0, CODES_END };
u16 actor1Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor2Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor3Conditions[] = { PROGRESS(0x16), 0, SPECIAL(0x22), 1, CODES_END };
u16 actor4Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor5Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor6Conditions[] = { PROGRESS(4), 1, CODES_END };
u16 actor7Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor8Conditions[] = { SPECIAL(0x15), 1, CODES_END };
u16 actor9Conditions[] = { PROGRESS(0xC), 1, CODES_END };
u16 actor10Conditions[] = { PROGRESS(0xE), 1, CODES_END };
u16 actor11Conditions[] = { SPECIAL(0x16), 1, CODES_END };
u16 actor12Conditions[] = { PROGRESS(0x16), 1, CODES_END };
u16 actor13Conditions[] = { SPECIAL(0x18), 1, CODES_END };
u16 actor14Conditions[] = { SPECIAL(0x19), 1, CODES_END };
u16 actor15Conditions[] = { PROGRESS(0x26), 1, CODES_END };
u16 actor16Conditions[] = { PROGRESS(0x16), 0, SPECIAL(0x22), 1, CODES_END };
u16 actor17Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor18Conditions[] = { SPECIAL(0x1E), 1, CODES_END };
u16 actor19Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor20Conditions[] = { SPECIAL(0x22), 1, CODES_END };
u16 actor21Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor22Conditions[] = { SPECIAL(0x22), 1, CODES_END };
u16 actor23Conditions[] = { PROGRESS(FIELD_PROGRESS_MOVIE_BATTLES), 1, CODES_END };
u16 actor24Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor25Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor26Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor27Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor28Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor29Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
u16 actor30Conditions[] = { SPECIAL(0x1A), 1, CODES_END };
FieldActorEntry actor0 = { actor0Conditions, actor0Talks, 0x20, 4, 187, 215, 1 };
FieldActorEntry actor1 = { actor1Conditions, actor1Talks, 0x20, 4, 187, 215, 1 };
FieldActorEntry actor2 = { actor2Conditions, actor2Talks, 0x20, 4, 187, 215, 1 };
FieldActorEntry actor3 = { actor3Conditions, actor3Talks, 0x24, 5, 220, 232, 1 };
FieldActorEntry actor4 = { actor4Conditions, actor4Talks, 0x24, 5, 220, 232, 1 };
FieldActorEntry actor5 = { actor5Conditions, actor5Talks, 0x24, 5, 220, 232, 1 };
FieldActorEntry actor6 = { actor6Conditions, actor6Talks, 0x32, 6, 65, 263, 7 };
FieldActorEntry actor7 = { actor7Conditions, actor7Talks, 0x32, 6, 65, 263, 7 };
FieldActorEntry actor8 = { actor8Conditions, actor8Talks, 0x32, 6, 65, 263, 7 };
FieldActorEntry actor9 = { actor9Conditions, actor9Talks, 0x32, 6, 65, 263, 7 };
FieldActorEntry actor10 = { actor10Conditions, actor10Talks, 0x32, 6, 65, 263, 7 };
FieldActorEntry actor11 = { actor11Conditions, actor11Talks, 0x32, 6, 65, 263, 7 };
FieldActorEntry actor12 = { actor12Conditions, actor12Talks, 0x32, 6, 65, 263, 7 };
FieldActorEntry actor13 = { actor13Conditions, actor13Talks, 0x32, 6, 65, 263, 7 };
FieldActorEntry actor14 = { actor14Conditions, actor14Talks, 0x32, 6, 65, 263, 7 };
FieldActorEntry actor15 = { actor15Conditions, actor15Talks, 0x32, 6, 65, 263, 7 };
FieldActorEntry actor16 = { actor16Conditions, actor16Talks, 0x43, 7, 399, 248, 1 };
FieldActorEntry actor17 = { actor17Conditions, actor17Talks, 0x43, 7, 399, 248, 1 };
FieldActorEntry actor18 = { actor18Conditions, NULL, 0x59, 8, 300, 217, 5 };
FieldActorEntry actor19 = { actor19Conditions, NULL, 0x59, 8, 300, 217, 5 };
FieldActorEntry actor20 = { actor20Conditions, NULL, 0x70, 9, 169, 224, 0 };
FieldActorEntry actor21 = { actor21Conditions, NULL, 0x70, 9, 169, 224, 0 };
FieldActorEntry actor22 = { actor22Conditions, NULL, 0x71, 0xA, 202, 241, 0 };
FieldActorEntry actor23 = { actor23Conditions, NULL, 0x71, 0xA, 202, 241, 0 };
FieldActorEntry actor24 = { actor24Conditions, actor24Talks, 0x9D, 0xB, 187, 215, 1 };
FieldActorEntry actor25 = { actor25Conditions, actor25Talks, 0x9E, 0xC, 220, 232, 1 };
FieldActorEntry actor26 = { actor26Conditions, actor26Talks, 0x9F, 0xD, 399, 248, 1 };
FieldActorEntry actor27 = { actor27Conditions, NULL, 0xA0, 0xE, 300, 217, 5 };
FieldActorEntry actor28 = { actor28Conditions, actor28Talks, 0xA1, 0xF, 65, 263, 7 };
FieldActorEntry actor29 = { actor29Conditions, NULL, 0x10E, 0x10, 169, 224, 0 };
FieldActorEntry actor30 = { actor30Conditions, NULL, 0x10F, 0x11, 202, 241, 0 };
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
    NULL,
};
StageTile stageObjects[] = {
    { 1, 0, 0x40, 2, 0x45, 2, 0, 2, 0xA, 0, 376, 237, 0, 0 },
    { 1, 0, 0x40, 2, 5, 0, 0, 0, 0, 0, 368, 277, 0, 0 },
    { 1, 0, 0x40, 2, 6, 0, 0, 0, 0, 0, 0, 204, 0, 0 },
    { 1, 0, 0x40, 2, 0x46, 2, 0, 1, 4, 0, 38, 197, 0, 0 },
    { 1, 0, 0x40, 6, 0x1F, 1, 0x1F, 0x24, 8, 0, 110, 341, 0, 0 },
    { 1, 0, 0x40, 6, 0x1F, 1, 0x1F, 0x24, 8, 0, 198, 149, 0, 0 },
    { 1, 0, 0x40, 6, 0x1F, 1, 0x1F, 0x24, 8, 0, 221, 148, 0, 0 },
    { 1, 0, 0x40, 6, 0x1F, 1, 0x1F, 0x24, 8, 0, 235, 149, 0, 0 },
    { 1, 0, 0x40, 6, 0x1F, 1, 0x1F, 0x24, 8, 0, 249, 297, 0, 0 },
    { 1, 0, 0x40, 6, 0x1F, 1, 0x1F, 0x24, 8, 0, 271, 168, 0, 0 },
    { 1, 0, 0x40, 6, 0x1F, 1, 0x1F, 0x24, 8, 0, 398, 350, 0, 0 },
    { 1, 0, 0x40, 6, 0x25, 1, 0x25, 0x2A, 8, 0, 102, 333, 0, 0 },
    { 1, 0, 0x40, 6, 0x25, 1, 0x25, 0x2A, 8, 0, 200, 134, 0, 0 },
    { 1, 0, 0x40, 6, 0x25, 1, 0x25, 0x2A, 8, 0, 213, 145, 0, 0 },
    { 1, 0, 0x40, 6, 0x25, 1, 0x25, 0x2A, 8, 0, 242, 167, 0, 0 },
    { 1, 0, 0x40, 6, 0x25, 1, 0x25, 0x2A, 8, 0, 248, 230, 0, 0 },
    { 1, 0, 0x40, 6, 0x25, 1, 0x25, 0x2A, 8, 0, 254, 280, 0, 0 },
    { 1, 0, 0x40, 6, 0x25, 1, 0x25, 0x2A, 8, 0, 265, 153, 0, 0 },
    { 1, 0, 0x40, 6, 0x25, 1, 0x25, 0x2A, 8, 0, 391, 343, 0, 0 },
    { 1, 0, 0x40, 6, 0x2B, 0, 0, 0, 0, 0, 213, 151, 0, 0 },
    { 1, 0, 0x40, 6, 0x2B, 0, 0, 0, 0, 0, 248, 286, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 0, 0, 0, 0, 0, 104, 328, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 0, 0, 0, 0, 0, 197, 141, 0, 0 },
    { 1, 0, 0x40, 6, 0x2C, 0, 0, 0, 0, 0, 264, 158, 0, 0 },
    { 1, 0, 0x40, 6, 0x2D, 0, 0, 0, 0, 0, 236, 159, 0, 0 },
    { 1, 0, 0x40, 6, 0x2D, 0, 0, 0, 0, 0, 391, 343, 0, 0 },
    { 1, 0, 0x40, 6, 0x32, 1, 0x32, 0x39, 0xE, 0, 318, 172, 0, 0 },
    { 1, 0, 0x40, 6, 0x2F, 1, 0x2F, 0x31, 4, 0, 305, 193, 0, 0 },
    { 1, 0, 0x40, 6, 0x3B, 1, 0x3B, 0x3F, 8, 0, 354, 209, 0, 0 },
    { 1, 0, 0x40, 6, 0x40, 2, 0, 9, 8, 0, 336, 217, 0, 0 },
    { 1, 0, 0x40, 6, 0x41, 2, 0, 2, 0x10, 0, 328, 232, 0, 0 },
    { 1, 0x64, 0x40, 6, 2, 0, 0, 0, 0, 0, 79, 186, 0, 0 },
    { 1, 0, 0x40, 6, 0x42, 2, 0, 2, 6, 0, 136, 185, 0, 0 },
    { 1, 0, 0x40, 6, 0x43, 2, 0, 2, 0x10, 0, 306, 231, 0, 0 },
    { 1, 0, 0x40, 6, 0x44, 2, 0, 2, 0xA, 0, 321, 223, 0, 0 },
    { 1, 0, 0x40, 6, 0x47, 2, 0, 1, 4, 0, 140, 123, 0, 0 },
    { 1, 0, 0x40, 6, 0x48, 2, 0, 1, 4, 0, 307, 122, 0, 0 },
    { 1, 0, 0x40, 6, 0x49, 2, 0, 1, 4, 0, 346, 144, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x4A, 1, 0x4A, 0x4C, 0xA, 0, 273, 297, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x56, 1, 0x56, 0x58, 0xA, 0, 215, 300, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x56, 1, 0x56, 0x58, 0xA, 0, 368, 358, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x59, 1, 0x59, 0x5B, 0xA, 0, 92, 347, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x59, 1, 0x59, 0x5B, 0xA, 0, 135, 312, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x59, 1, 0x59, 0x5B, 0xA, 0, 150, 375, 0, 0 },
    { 1, 0, 0x40, 0xA, 0x59, 1, 0x59, 0x5B, 0xA, 0, 418, 326, 0, 0 },
    { 1, 0, 0x40, 4, 0, 0, 0, 0, 0, 0, 143, 287, 340, 0 },
    { 1, 0, 0x40, 4, 1, 0, 0, 0, 0, 0, 326, 288, 340, 0 },
    { 1, 0, 0x40, 4, 4, 0, 0, 0, 0, 0, 398, 240, 263, 0 },
    { 1, 0, 0x40, 4, 3, 0, 0, 0, 0, 0, 33, 201, 239, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageSlot stageSlots[] = {
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x203, 0x2C8, 0x24C, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x200, 0x410, 0x200, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageFuncs stageFuncs = { setupStage, startTween, updateTween };
FieldEvent stageEvents[] = {
    { 53, script53, EVENT_TEXT(0xD), NULL, NULL },
    { 54, script54, EVENT_TEXT(0xE), NULL, NULL },
    { 55, script55, EVENT_TEXT(0xF), NULL, NULL },
    { 56, script56, EVENT_TEXT(0x10), NULL, NULL },
    { 1512, NULL, EVENT_TEXT(0x36), startEvent1512, NULL },
    { 1513, script1513, EVENT_TEXT(0x31), NULL, NULL },
    { 1514, NULL, EVENT_TEXT(0x37), startEvent1514, NULL },
    { 1515, script1515, EVENT_TEXT(0x32), NULL, NULL },
    { 1516, NULL, EVENT_TEXT(0x38), startEvent1516, NULL },
    { 1517, script1517, EVENT_TEXT(0x33), NULL, NULL },
    { -1, NULL, 0, NULL, NULL },
};
