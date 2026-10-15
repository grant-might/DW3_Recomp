/* The slot picker: its header and the memory card slots, which slide in and
   out */

#include "menus/memory_card.h"

/* Slides the slot picker's header down into view */
void STGMCARD_slideInHeader(MemCardMenu *menu) {
    TextWindow **windows = menu->children;

    STGMCARD_funcs.startLerp(&menu->lerps[1], -0x55, 0, 10);
    windows[0]->setNumber(windows[0], 1, menu->saves->port + 1);
#if VERSION_EU
    windows[0]->setVisible(windows[0], 0);
#endif
    menu->substate = 1;
}

/* Selects `slot`, slides the slot picker's cursor into place and moves it to the slot;
   then the player picks one with left and right */
void STGMCARD_startSlotPick(MemCardMenu *menu, s32 slot) {
    STGMCARD_funcs.startLerp(&menu->lerps[2], 0, 0x2F, 8);
    menu->substate = 5;
    STGMCARD_funcs.slot = slot;
    STGMCARD_funcs.prevSlot = 0;
}

/* Slides the slot picker's header up out of view, then resets the picker */
void STGMCARD_slideOutHeader(MemCardMenu *menu) {
    STGMCARD_funcs.startLerp(&menu->lerps[1], 0, -0x55, 5);
    menu->substate = 4;
}

/* Slides the slot picker's cursor from the previous slot to the selected one */
void STGMCARD_moveSlotCursor(MemCardMenu *menu) {
    STGMCARD_funcs.startLerp(&menu->lerps[3], STGMCARD_funcs.prevSlot * 0x44, STGMCARD_funcs.slot * 0x44, 5);
    menu->substate = 7;
}

/* Slides the slot picker's three slots in from the right */
void STGMCARD_slideInSlots(MemCardMenu *menu) {
    STGMCARD_funcs.startLerp(&menu->lerps[0], 0xDE, 0, 10);
    menu->substate = 2;
}

/* Slides the slot picker's three slots out to the right */
void STGMCARD_slideOutSlots(MemCardMenu *menu) {
    STGMCARD_funcs.startLerp(&menu->lerps[0], 0, 0xDE, 5);
    menu->substate = 3;
}

/* Puts the slot picker back out of view, idle, with slot 0 selected */
void STGMCARD_resetMenu(MemCardMenu *menu) {
    menu->substate = 0;
    menu->slid = 0;
    STGMCARD_funcs.slot = 0;
    STGMCARD_funcs.prevSlot = 0;
    STGMCARD_funcs.startLerp(&menu->lerps[0], 0xDE, 0, 10);
    STGMCARD_funcs.startLerp(&menu->lerps[1], -0x55, 0, 10);
    STGMCARD_funcs.startLerp(&menu->lerps[2], 0, 0x2F, 8);
    STGMCARD_funcs.startLerp(&menu->lerps[3], -1, 0, 1);
    menu->lerps[3].value = 0;
}

/* The save picker's task: slides in and out, moves between the first three
   saves with left and right, and draws each one's lead partner */
void STGMCARD_updateMenu(MemCardMenu *menu, TextWindow **windows) {
    SpriteDrawer sprite;
    s32 prev;
    s32 i;

    switch (menu->state) {
    case TASK_INIT:
    default:
        menu->nextState(menu);
        STGMCARD_resetMenu(menu);
        if (*windows == NULL) {
            *windows = createTextWindow(menu->layer, 1, 0x15, menu->lerps[1].value + 0x18);
        }
        (*windows)->setString(*windows, FILE_CACHE.load(TEXT_FILE(TEXT_MEMORY_CARD)), 3);
        (*windows)->setNumber(*windows, 1, menu->saves->port + 1);
#if VERSION_EU
        (*windows)->setVisible(*windows, 0);
#endif
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    case TASK_RUN:
        switch (menu->substate) {
        case 0:
            break;
        case 1:
            if (STGMCARD_funcs.updateLerp(&menu->lerps[1]) != 0) {
                menu->substate = 0;
                menu->slid = 1;
            }
            break;
        case 2:
            if (STGMCARD_funcs.updateLerp(&menu->lerps[0]) != 0) {
                menu->substate = 0;
                menu->slid = 2;
            }
            break;
        case 3:
            if (STGMCARD_funcs.updateLerp(&menu->lerps[0]) != 0) {
                menu->substate = 0;
                menu->slid = 3;
            }
            break;
        case 4:
            if (STGMCARD_funcs.updateLerp(&menu->lerps[1]) != 0) {
                STGMCARD_resetMenu(menu);
            }
            break;
        case 5:
            if (STGMCARD_funcs.updateLerp(&menu->lerps[2]) != 0) {
                STGMCARD_moveSlotCursor(menu);
            }
            break;
        case 6:
            if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
                prev = STGMCARD_funcs.slot;
                STGMCARD_funcs.prevSlot = prev;
                STGMCARD_funcs.slot = prev - 1;
                if (STGMCARD_funcs.slot < 0) {
                    STGMCARD_funcs.slot = 0;
                }
                if (STGMCARD_funcs.slot != prev) {
                    menu->saves->refresh(menu->saves);
                    STGMCARD_moveSlotCursor(menu);
                    SOUND.playSound(SOUND_MENU_MOVE);
                }
            } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
                prev = STGMCARD_funcs.slot;
                STGMCARD_funcs.slot = prev + 1;
                STGMCARD_funcs.prevSlot = prev;
                if (STGMCARD_funcs.slot >= 3) {
                    STGMCARD_funcs.slot = 2;
                }
                if (STGMCARD_funcs.slot != prev) {
                    menu->saves->refresh(menu->saves);
                    STGMCARD_moveSlotCursor(menu);
                    SOUND.playSound(SOUND_MENU_MOVE);
                }
            }
            break;
        case 7:
            if (STGMCARD_funcs.updateLerp(&menu->lerps[3]) != 0) {
                menu->substate = 6;
            }
            break;
        }
        initSpriteDrawer(&sprite);
        sprite.setLayerId(menu->layer, menu->depth);
        sprite.setTexture(0x280, 0);
        i = 0;
        if (GFX.funcs.getTime() - menu->blinkTime >= 3) {
            menu->blinkTime = GFX.funcs.getTime();
            if (menu->blinkDown == 0) {
                if (++menu->blinkFrame >= 12) {
                    menu->blinkFrame = 10;
                    menu->blinkDown = 1;
                }
            } else {
                if (--menu->blinkFrame <= 0) {
                    menu->blinkFrame = 0;
                    menu->blinkDown = 0;
                }
            }
        }
        sprite.setClutRow(menu->blinkFrame);
        sprite.draw(FILE_CACHE.getEntry(FILE_GMCARD_SHEET << 16), 0x23, menu->lerps[2].value + 0x30 + menu->lerps[3].value, menu->lerps[1].value + 0x12);
        sprite.setClutRow(0);
        sprite.draw(FILE_CACHE.getEntry(FILE_GMCARD_SHEET << 16), 0x20, 0, menu->lerps[1].value + 0x10);
        (*windows)->setPos(*windows, 0x15, menu->lerps[1].value + 0x18);
        for (i = 0; i < 3; i++) {
            if (menu->saves->file.saves[i].name[0] != 0) {
                sprite.draw(FILE_CACHE.getEntry(FILE_GMCARD_SHEET << 16), STGMCARD_partnerIcons[menu->saves->file.saves[i].partners[0] - 3], STGMCARD_slotIconX[i] + menu->lerps[0].value, 0x23);
            }
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_GMCARD_SHEET << 16), 0x22, menu->lerps[0].value + 0x62, 0x20);
        break;
    }
}

/* Creates the slot picker (task) of the save list */
MemCardMenu *STGMCARD_createMenu(MemCardSaves *saves) {
    MemCardMenu *menu = createTask(STGMCARD_updateMenu, sizeof(MemCardMenu), 4);

    menu->reset = STGMCARD_resetMenu;
    menu->slideInHeader = STGMCARD_slideInHeader;
    menu->startPick = STGMCARD_startSlotPick;
    menu->slideOutHeader = STGMCARD_slideOutHeader;
    menu->slideInSlots = STGMCARD_slideInSlots;
    menu->slideOutSlots = STGMCARD_slideOutSlots;
    menu->layer = SCREEN_LAYER;
    menu->depth = 2;
    menu->saves = saves;
    return menu;
}
