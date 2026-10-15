/*
 * WSTAG924: Digimon Lab, Amaterasu City, in the extra chapter. The European
 * version's: in its extra chapter (FIELD_PROGRESS_EXTRA), mode 629 starts it
 * instead of WSTAG221.
 */

#include "common.h"
#define STAGE_TWEEN /* stageFuncs is a StageFuncs (stage.h) */
#include "field/stage.h"
/* Defined below, after the code that uses them */
extern s32 event1602WindowFrames[];
extern s32 event1604WindowFrames[];

/* The text file of the menus */
#define MENU_TEXT EXTRA_TEXT

#include "common/update_stage.inc.c"

#include "common/start_stage.inc.c"

#if VERSION_EU
#define STAGE_TEXT (LANGUAGE + 0xFD)
#define STAGE_FILE 0x8E7
#define STAGE_FILE_8 0x19C
#define STAGE_ARCHIVE 0x8E5
#elif VERSION_JP
#define STAGE_TEXT 0x89A
#define STAGE_FILE 0x8A1
#define STAGE_FILE_8 0x18E
#define STAGE_ARCHIVE 0x8A0
#endif
/* Sets the stage up: its map, actors and events, playing the ambience ENV_0011 */
void setupStage(void) {
    FIELDSTG_state.textFile = STAGE_TEXT;
    FIELDSTG_state.mapFile = STAGE_FILE_8;
    FIELDSTG_state.sheetEntry = STAGE_FILE << 16;
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
    FIELDSTG_map.setFile(FIELD_MAP_FLOOR0, STAGE_FILE << 16 | 1);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, STAGE_FILE << 16 | 2);
    FIELDSTG_map.setFirstMap(0);
}

#include "common/start_tween.inc.c"
#include "common/update_tween.inc.c"

#include "common/list_menu.inc.c"

/* A list of up to eight options that each open a message */
void updateEvent1602(StageListMenu *task, StageListMenuChildren *children) {
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
            drawer.draw(FILE_CACHE.getEntry(MENU_SPRITES), event1602WindowFrames[task->count - 5], 0xA8, 0x18);
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
 * Starts event 1602, the starters' digivolutions: a list of Kotemon, Monmon,
 * Kumamon, Renamon, Patamon, Agumon and Guilmon that each open a message
 */
void *startEvent1602(void) {
    return createTask(updateEvent1602, sizeof(StageListMenu), sizeof(StageListMenuChildren));
}

/* A list of up to eight options that each open a message */
void updateEvent1604(StageListMenu *task, StageListMenuChildren *children) {
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
                showListMenuOptions(task, children, 0x2);
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
                openListMenuMessage(task, children, 0x2);
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
            drawer.draw(FILE_CACHE.getEntry(MENU_SPRITES), event1604WindowFrames[task->count - 5], 0xA8, 0x18);
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
 * Starts event 1604, tips on raising partners: a list of topics (Raise
 * efficiently, Techniques, Growth limits...) that each open a message
 */
void *startEvent1604(void) {
    return createTask(updateEvent1604, sizeof(StageListMenu), sizeof(StageListMenuChildren));
}

ActorImage stageImages[] = {
    { 0x200, 0x100, 0x21C, 0x1A6, 0x70, 0xA6, 0x230, 0x1FE },
    { 0x200, 0x100, 0x200, 0x100, 0, 0, 0x220, 0x1FE },
    { 0x200, 0x100, 0x216, 0x138, 0x58, 0x38, 0x200, 0x1FD },
    { 0x200, 0x100, 0x208, 0x1BC, 0x20, 0xBC, 0x210, 0x1FD },
    { 0x200, 0x100, 0x210, 0x1BC, 0x40, 0xBC, 0x220, 0x1FD },
    { 0x200, 0x100, 0x200, 0x1BC, 0, 0xBC, 0x230, 0x1FD },
    { 0x140, 0x100, 0x166, 0x178, 0x98, 0x78, 0x150, 0x1F8 },
    { 0x140, 0x100, 0x16E, 0x178, 0xB8, 0x78, 0x160, 0x1F8 },
    { 0x140, 0x100, 0x176, 0x178, 0xD8, 0x78, 0x170, 0x1F8 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x140, 0x100, 0x152, 0x166, 0x48, 0x66, 0x150, 0x1F7 },
    { 0x140, 0x100, 0x140, 0x17E, 0, 0x7E, 0x160, 0x1F7 },
};
u16 actor0Talk0Actions[] = { START_EVENT(0x6E), 1, CODES_END };
u16 actor1Talk0Actions[] = { START_EVENT(0x6D), 1, CODES_END };
u16 actor5Talk0Actions[] = { 0x7C00, 1, CODES_END };
FieldTalk actor0Talks[] = {
    { NULL, actor0Talk0Actions, 0x23 },
    { NULL, NULL, 0 },
};
FieldTalk actor1Talks[] = {
    { NULL, actor1Talk0Actions, 0x22 },
    { NULL, NULL, 0 },
};
FieldTalk actor5Talks[] = {
    { NULL, actor5Talk0Actions, 0x24 },
    { NULL, NULL, 0 },
};
FieldTalk actor6Talks[] = {
    { NULL, NULL, 0x25 },
    { NULL, NULL, 0 },
};
FieldActorEntry actor0 = { NULL, actor0Talks, 0x20, 4, 220, 232, 1 };
FieldActorEntry actor1 = { NULL, actor1Talks, 0x24, 5, 187, 215, 1 };
FieldActorEntry actor2 = { NULL, NULL, 0x59, 6, 300, 217, 5 };
FieldActorEntry actor3 = { NULL, NULL, 0x70, 7, 202, 241, 1 };
FieldActorEntry actor4 = { NULL, NULL, 0x71, 8, 169, 224, 1 };
FieldActorEntry actor5 = { NULL, actor5Talks, 0xFE, 9, 399, 248, 1 };
FieldActorEntry actor6 = { NULL, actor6Talks, 0x177, 0xA, 65, 263, 1 };
FieldActorEntry *stageActors[] = {
    &actor0,
    &actor1,
    &actor2,
    &actor3,
    &actor4,
    &actor5,
    &actor6,
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
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x273, 0x2C8, 0x24C, 3, 0x64, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, SLOT_EXIT, 0x270, 0x410, 0x200, 1, 0, 0, 0 },
    { { { CODES_END, 0 }, { CODES_END, 0 } }, 0, 0, 0, 0, 0, 0, 0, 0 },
};
StageFuncs stageFuncs = { setupStage, startTween, updateTween };
#define EVENT_TEXT_FILE EXTRA_TEXT
FieldEvent stageEvents[] = {
    { 1602, NULL, EVENT_TEXT(1), startEvent1602, NULL },
    { 1604, NULL, EVENT_TEXT(2), startEvent1604, NULL },
    { -1, NULL, 0, NULL, NULL },
};
/* MENU_SPRITES' frame of the list's window, by the number of options (5 to 8) */
s32 event1602WindowFrames[] = {
    28, 27, 25, 36,
};
/* MENU_SPRITES' frame of the list's window, by the number of options (5 to 8) */
s32 event1604WindowFrames[] = {
    28, 27, 25, 36,
};
