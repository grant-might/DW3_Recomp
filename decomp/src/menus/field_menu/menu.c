/* The mode's main task, which runs the field menu's screens; the scroll bar;
   the files it loads; and the panels' openings and moves */

#include "menus/field_menu.h"

#include "shared/set_scroll_bar_x.inc.c"
#include "shared/set_scroll_bar_range.inc.c"
#include "shared/set_scroll_bar_count.inc.c"
#include "shared/set_scroll_bar_pos.inc.c"
#include "shared/update_scroll_bar.inc.c"
#define SCROLL_BAR_DEPTH 0
#include "shared/create_scroll_bar.inc.c"

/* Opens the screen of FIELD_MENU_CHOICE (the item screen when it has none) and,
   once it closes, the field menu again */
void STSTATUS_runScreens(FieldMenuScreen *menu, FieldMenuScreenChildren *children) {
    Task *(*open)(FieldMenuScreen *, s32);

    switch (menu->substate) {
    case 0:
    default:
        open = STSTATUS_screens[FIELD_MENU_CHOICE.extra][FIELD_MENU_CHOICE.option];
        if (open != NULL) {
            children->screen = open(menu, FIELD_MENU_CHOICE.extra);
        } else {
            children->screen = STSTATUS_createItemScreen(menu, FIELD_MENU_CHOICE.extra);
        }
        menu->substate++;
        break;
    case 1:
        if (children->screen == NULL) {
            children->fieldMenu = createFieldMenu(menu->layer, FIELD_MENU_CHOICE.option);
            menu->setState(menu, TASK_DONE);
        }
        break;
    }
}

/* Draws the scrolling background, sprite 0x1D at (bgScroll, bgScroll): it
   moves one pixel down and right every other frame, back to 0 at 0x60 */
void STSTATUS_drawBackground(FieldMenuScreen *menu) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(menu->layer, 7);
    sprite.setTexture(0x280, 0x100);
    if (menu->bgTick != 0) {
        menu->bgScroll++;
        menu->bgScroll = menu->bgScroll < 0x60 ? menu->bgScroll : 0;
        menu->bgTick = 0;
    } else {
        menu->bgTick = 1;
    }
    sprite.draw(FILE_CACHE.getEntry(FILE_STATUS_SPRITES << 16), 0x1D, menu->bgScroll, menu->bgScroll);
}

/* The mode's main task: loads the files and the background's textures, then runs
   the screens (STSTATUS_runScreens); while the field menu is open (TASK_DONE) it
   waits for it to close */
void STSTATUS_updateMenu(FieldMenuScreen *menu, FieldMenuScreenChildren *children) {
    TimLoader loader;

    switch (menu->state) {
    case TASK_INIT:
    default:
        switch (menu->substate) {
        case 0:
        default:
            STSTATUS_data.funcs.loadFiles();
            menu->substate++;
            break;
        case 1:
            if (STSTATUS_data.funcs.filesLoading() == 0 && FILE_CACHE.isLoading(menu->bgFile) == 0 &&
                FILE_CACHE.isLoading(menu->bgFile2) == 0) {
                initTimLoader(&loader);
                loader.setImagePos(0x380, 0x100);
                loader.loadArchive(FILE_CACHE.getEntry(menu->bgArchive1));
                loader.setImagePos(0x300, 0x100);
                loader.loadArchive(FILE_CACHE.getEntry(menu->bgArchive2));
                loader.setImagePos(0x280, 0);
                loader.setClutPos(0x140, 0x100);
                loader.loadArchive(FILE_CACHE.getEntry(menu->bgArchive));
                menu->nextState(menu);
            }
            break;
        }
        break;
    case TASK_RUN:
        STSTATUS_runScreens(menu, children);
        STSTATUS_drawBackground(menu);
        break;
    case TASK_DONE:
        if (children->fieldMenu == NULL) {
            menu->state = TASK_RUN;
        }
        STSTATUS_drawBackground(menu);
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates the mode's main task and requests its background's files, the second
   half of the game's in the late game */
FieldMenuScreen *STSTATUS_createMenu(void) {
    FieldMenuScreen *menu = createTask(STSTATUS_updateMenu, sizeof(FieldMenuScreen), sizeof(FieldMenuScreenChildren));

    menu->layer = SCREEN_LAYER;
    menu->gameHalf = STSTATUS_areaFuncs.getGameHalf();
    if (menu->gameHalf == 0) {
        menu->bgArchive = (FILE_STATUS_BG + 1) << 16;
        menu->bgFile = FILE_STATUS_BG + 1;
        menu->bgArchive1 = ((FILE_STATUS_BG + 1) << 16) + 1;
        menu->bgArchive2 = ((FILE_STATUS_BG + 1) << 16) + 2;
        menu->bgFile2 = FILE_STATUS_BG;
    } else {
        menu->bgArchive = (FILE_STATUS_BG + 3) << 16;
        menu->bgFile = FILE_STATUS_BG + 3;
        menu->bgArchive1 = ((FILE_STATUS_BG + 3) << 16) + 1;
        menu->bgArchive2 = ((FILE_STATUS_BG + 3) << 16) + 2;
        menu->bgFile2 = FILE_STATUS_BG + 2;
    }
    FILE_CACHE.request(menu->bgFile);
    FILE_CACHE.request(menu->bgFile2);
    return menu;
}

/* Loads the menu's textures and requests its strings: the menu's, the items',
   the Digimon's and the skills' names and descriptions */
void STSTATUS_loadFiles(void) {
    TimLoader loader;

    initTimLoader(&loader);
    loader.setImagePos(0x280, 0x100);
    loader.loadArchive(FILE_CACHE.getEntry((FILE_STATUS_SPRITES + 1) << 16));
    FILE_CACHE.request(TEXT_FILE(TEXT_STATUS));
    FILE_CACHE.request(TEXT_FILE(TEXT_ITEM_NAMES));
    FILE_CACHE.request(TEXT_FILE(TEXT_ITEM_INFO));
    FILE_CACHE.request(TEXT_FILE(TEXT_DIGIMON_NAMES));
    FILE_CACHE.request(TEXT_FILE(TEXT_DIGIMON_INFO));
    FILE_CACHE.request(TEXT_FILE(TEXT_SKILL_NAMES));
    FILE_CACHE.request(TEXT_FILE(TEXT_SKILL_INFO));
}

/* Whether the menu's strings are still loading */
s32 STSTATUS_filesLoading(void) {
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_STATUS)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_ITEM_NAMES)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_ITEM_INFO)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_DIGIMON_NAMES)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_DIGIMON_INFO)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_SKILL_NAMES)) != 0) {
        return 1;
    }
    return FILE_CACHE.isLoading(TEXT_FILE(TEXT_SKILL_INFO)) != 0;
}

#include "shared/start_fade.inc.c"
#include "shared/update_fade.inc.c"

/* Starts moving a value from FROM to TO in FRAMES frames, playing SOUND_MENU_OPEN;
   nothing when they are the same */
void STSTATUS_startLerp(MenuLerp *lerp, s32 from, s32 to, s32 frames) {
    if (from != to) {
        SOUND.playSound(SOUND_MENU_OPEN);
        lerp->duration = frames;
        lerp->fixed = from << 8;
        lerp->value = from;
        lerp->target = to;
        lerp->active = 1;
        lerp->step = ((to - from) << 8) / lerp->duration;
    }
}

#include "shared/update_lerp.inc.c"
