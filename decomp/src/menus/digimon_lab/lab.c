/* The mode's main task, the lab, which runs the main menu and its screens;
   its files and helpers; and the overlay's tables */

#include "menus/digimon_lab.h"

/* Creates the main menu and opens the screen it picks, back to the menu when
   the screen closes; once the menu is gone, ends the lab after the fade out */
void STGDGLAB_runLab(Lab *lab, LabChildren *children) {
    switch (lab->substate) {
    case 0:
    default:
        if (children->menu == NULL) {
            children->menu = STGDGLAB_createMenu(lab);
        }
        lab->substate++;
        break;
    case 1:
        if (children->menu != NULL) {
            if (children->menu->picked != 0) {
                children->screen = STGDGLAB_screens[children->menu->choice](lab);
                lab->substate++;
            }
        } else {
            lab->substate = 3;
        }
        break;
    case 2:
        if (children->screen == NULL) {
            lab->substate = 1;
            children->menu->picked = 0;
        }
        break;
    case 3:
        if (children->fade->state == 2) {
            lab->setState(lab, TASK_KILL);
        }
        break;
    }
}

/* Counts the party and moves its members to the first places of GAME.party */
void STGDGLAB_packParty(Lab *lab) {
    s32 i;
    s32 j;

    lab->partyCount = 0;
    for (i = 0; i < 3; i++) {
        if (GAME.party[i] >= 0) {
            lab->partyCount++;
        }
    }
    for (i = 0; i < 3; i++) {
        if (GAME.party[i] < 0) {
            for (j = i; j < 3; j++) {
                if (GAME.party[j] >= 0) {
                    GAME.party[i] = GAME.party[j];
                    GAME.party[j] = -1;
                    break;
                }
            }
        }
    }
}

/* The mode's main task: loads the files and packs the party, then runs the lab
   (STGDGLAB_runLab) and draws sprite 0x37 sliding diagonally; when killed it
   goes back to the field (GAME.fieldMode) */
void STGDGLAB_updateLab(Lab *lab, LabChildren *children) {
    SpriteDrawer sprite;

    switch (lab->state) {
    case TASK_INIT:
    default:
        switch (lab->substate) {
        case 0:
        default:
            STGDGLAB_data.funcs.loadFiles();
            lab->substate++;
            break;
        case 1:
            if (STGDGLAB_data.funcs.filesLoading() == 0) {
                lab->nextState(lab);
                lab->partySize = PARTY_SIZE;
                STGDGLAB_packParty(lab);
            }
            break;
        }
        break;
    case TASK_RUN:
        STGDGLAB_runLab(lab, children);
        initSpriteDrawer(&sprite);
        sprite.setLayerId(lab->layer, 7);
        sprite.setTexture(0x280, 0x100);
        if (lab->bgTick != 0) {
            lab->bgScroll++;
            lab->bgScroll = lab->bgScroll < 0x60 ? lab->bgScroll : 0;
            lab->bgTick = 0;
        } else {
            lab->bgTick = 1;
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_LAB_SPRITES << 16), 0x37, lab->bgScroll, lab->bgScroll);
        break;
    case TASK_DONE:
        break;
    case TASK_KILL:
        GAME.funcs.requestMode(GAME.fieldMode, 0);
        break;
    }
}

/* Lab.openMenu: opens the main menu's page again; 1 if the menu was running */
s32 STGDGLAB_openLabMenu(Lab *lab) {
    LabChildren *children = lab->children;
    LabMenu *menu = children->menu;

    if (menu != NULL && menu->state == TASK_RUN) {
        menu->open(menu);
        return 1;
    }
    return 0;
}

/* Lab.closeMenu: closes the main menu's page; 1 if the menu was running */
s32 STGDGLAB_closeLabMenu(Lab *lab) {
    LabChildren *children = lab->children;
    LabMenu *menu = children->menu;

    if (menu != NULL && menu->state == TASK_RUN) {
        menu->close(menu);
        return 1;
    }
    return 0;
}

/* Lab.menuOpen: 1 when the main menu's task is running (its page not fading) */
s32 STGDGLAB_labMenuRunning(Lab *lab) {
    LabChildren *children = lab->children;
    LabMenu *menu = children->menu;

    if (menu != NULL && menu->state == TASK_RUN) {
        return 1;
    }
    return 0;
}

/* Lab.fadeOut: fades the screen out in 0x1E frames, as the lab is left */
void STGDGLAB_fadeOutLab(Lab *lab) {
    LabChildren *children = lab->children;

    children->fade = STGDGLAB_createFader();
    children->fade->start(children->fade, 0, 0x1E);
}

/* Creates the mode's main task, the lab */
Lab *STGDGLAB_createLab(void) {
    Lab *lab = createTask(STGDGLAB_updateLab, sizeof(Lab), sizeof(LabChildren));

    lab->openMenu = STGDGLAB_openLabMenu;
    lab->closeMenu = STGDGLAB_closeLabMenu;
    lab->menuOpen = STGDGLAB_labMenuRunning;
    lab->packParty = STGDGLAB_packParty;
    lab->fadeOut = STGDGLAB_fadeOutLab;
    lab->layer = SCREEN_LAYER;
    return lab;
}

/* Loads the lab's textures and requests its strings: the lab's and the
   Digimon's and skills' names and descriptions */
void STGDGLAB_loadFiles(void) {
    TimLoader loader;

    initTimLoader(&loader);
    loader.setImagePos(0x280, 0x100);
    loader.loadArchive(FILE_CACHE.getEntry((FILE_LAB_SPRITES + 1) << 16));
    loader.setImagePos(0x140, 0x100);
    loader.setClutPos(0x280, 0);
    loader.setBufferSize(0x10000);
    loader.loadArchive(FILE_CACHE.getEntry(((FILE_LAB_SPRITES + 1) << 16) + 2));
    FILE_CACHE.request(TEXT_FILE(TEXT_DIGI_LAB));
    FILE_CACHE.request(TEXT_FILE(TEXT_DIGIMON_NAMES));
    FILE_CACHE.request(TEXT_FILE(TEXT_DIGIMON_INFO));
    FILE_CACHE.request(TEXT_FILE(TEXT_SKILL_NAMES));
    FILE_CACHE.request(TEXT_FILE(TEXT_SKILL_INFO));
}

/* Whether the lab's strings are still loading */
s32 STGDGLAB_filesLoading(void) {
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_DIGI_LAB)) != 0) {
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
#include "shared/start_lerp.inc.c"
#include "shared/update_lerp.inc.c"

/* The sprite of a recipe's id (STGDGLAB_entries), 0 for an id not there */
s32 STGDGLAB_getItemSprite(s32 id) {
    s32 i;

    for (i = 0; STGDGLAB_entries[i].id != 0; i++) {
        if (STGDGLAB_entries[i].id == id) {
            return STGDGLAB_entries[i].sprite;
        }
    }
    return 0;
}

/* The third value (b) of a recipe's id in STGDGLAB_entries, 0 for an id not
   there; LabFuncs.getB, which nothing in the overlay calls */
s32 STGDGLAB_getItemB(s32 id) {
    s32 i;

    for (i = 0; STGDGLAB_entries[i].id != 0; i++) {
        if (STGDGLAB_entries[i].id == id) {
            return STGDGLAB_entries[i].b;
        }
    }
    return 0;
}

/* The main menu's screens */
Task *(*STGDGLAB_screens[])(Lab *lab) = {
    STGDGLAB_createPartyScreen, STGDGLAB_createSlotScreen, STGDGLAB_createRecipeScreen,
};
/* The partners' animation frames, -1 ends */
PartnerAnim STGDGLAB_partnerAnims[] = {
    { { 7, 8, 9, 10, 9, 8, -1 } },
    { { 14, 15, 16, 15, -1, -1, -1 } },
    { { 11, 12, 13, 12, -1, -1, -1 } },
    { { 3, 4, 5, 6, 5, 4, -1 } },
    { { 25, 26, 27, 28, 27, 26, -1 } },
    { { 0, 1, 2, 1, -1, -1, -1 } },
    { { 17, 18, 19, 20, 19, 18, -1 } },
    { { 21, 22, 23, 24, 23, 22, -1 } },
};
/* Where the windows go: the five stats' labels, their values and the name */
LabWindowPos STGDGLAB_layout[] = {
    { 21, 51, 38 },
    { 22, 51, 48 },
    { 23, 95, 48 },
    { 14, 51, 57 },
    { 23, 95, 57 },
    { 18, 80, 38 },
    { 18, 93, 48 },
    { 18, 128, 48 },
    { 18, 93, 57 },
    { 18, 128, 57 },
    { -1, 51, 23 },
};
/* The items of the recipes, up to id 0: their sprite (STGDGLAB_getItemSprite) and
   STGDGLAB_getItemB's value */
LabEntry STGDGLAB_entries[] = {
    { 0x017F, 0x0002, 0x010C },
    { 0x0181, 0x0004, 0x011D },
    { 0x0180, 0x0003, 0x0114 },
    { 0x0003, 0x0001, 0x012A },
    { 0x0091, 0x0007, 0x012B },
    { 0x016E, 0x0000, 0x012C },
    { 0x0175, 0x0005, 0x0115 },
    { 0x001F, 0x0006, 0x010D },
    { 0x0005, 0x0022, 0x0127 },
    { 0x0006, 0x001D, 0x0132 },
    { 0x000C, 0x0023, 0x0125 },
    { 0x0013, 0x0017, 0x011C },
    { 0x0014, 0x000B, 0x010A },
    { 0x001A, 0x0028, 0x0131 },
    { 0x001B, 0x0025, 0x0108 },
    { 0x0038, 0x0021, 0x0135 },
    { 0x003B, 0x0010, 0x0123 },
    { 0x0042, 0x001E, 0x0130 },
    { 0x0090, 0x000F, 0x0119 },
    { 0x0094, 0x0013, 0x0121 },
    { 0x0096, 0x002A, 0x011E },
    { 0x0097, 0x0019, 0x012D },
    { 0x00C4, 0x0026, 0x0117 },
    { 0x00D3, 0x000C, 0x0105 },
    { 0x00D5, 0x0024, 0x0102 },
    { 0x00D6, 0x000D, 0x0134 },
    { 0x00E6, 0x0018, 0x0118 },
    { 0x00EA, 0x000E, 0x0106 },
    { 0x00FE, 0x0012, 0x0124 },
    { 0x0103, 0x0011, 0x0129 },
    { 0x0104, 0x0016, 0x010B },
    { 0x010B, 0x0029, 0x0133 },
    { 0x0167, 0x0014, 0x0120 },
    { 0x016F, 0x0008, 0x0128 },
    { 0x0170, 0x0009, 0x0126 },
    { 0x0171, 0x000A, 0x011F },
    { 0x0174, 0x0027, 0x0122 },
    { 0x0176, 0x001A, 0x0112 },
    { 0x0177, 0x001B, 0x0110 },
    { 0x0178, 0x001C, 0x010F },
    { 0x0179, 0x0020, 0x012F },
    { 0x017A, 0x001F, 0x012E },
    { 0x017D, 0x0015, 0x0100 },
    { 0x0182, 0x0031, 0x0109 },
    { 0x0183, 0x002B, 0x0113 },
    { 0x0184, 0x002E, 0x011B },
    { 0x0185, 0x0032, 0x0107 },
    { 0x0186, 0x002C, 0x0111 },
    { 0x0187, 0x002F, 0x011A },
    { 0x0188, 0x0033, 0x0101 },
    { 0x0189, 0x002D, 0x010E },
    { 0x018A, 0x0030, 0x0116 },
    { 0x0000, 0x0000, 0x0000 },
};
/* The recipes of each table (STGDGLAB_data.recipes), by row and column */
#if VERSION_US
LabRecipe STGDGLAB_recipes0[] = {
    { 3, { 0x182, 0x185, 0x188, 0, 0 } },
    { 3, { 0x005, 0x00C, 0x0D5, 0, 0 } },
    { 3, { 0, 0x01A, 0x10B, 0x096, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 5, { 0x103, 0x0FE, 0x094, 0x167, 0x17D } },
    { 3, { 0x01B, 0x0C4, 0x174, 0, 0 } },
    { 4, { 0x104, 0x013, 0, 0x0E6, 0x097 } },
    { 3, { 0x176, 0x177, 0x178, 0, 0 } },
    { 3, { 0x0EA, 0x090, 0x03B, 0, 0 } },
    { 3, { 0x014, 0x0D3, 0x0D6, 0, 0 } },
    { 1, { 0x038, 0, 0, 0, 0 } },
    { 4, { 0x006, 0x042, 0x17A, 0x179, 0 } },
    { 3, { 0x16F, 0x170, 0x171, 0, 0 } },
    { 3, { 0x183, 0x186, 0x189, 0, 0 } },
    { 3, { 0x184, 0x187, 0x18A, 0, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
};
LabRecipe STGDGLAB_recipes1[] = {
    { 3, { 0x184, 0x187, 0x18A, 0, 0 } },
    { 3, { 0x176, 0x177, 0x178, 0, 0 } },
    { 1, { 0x038, 0, 0, 0, 0 } },
    { 5, { 0x103, 0x0FE, 0x094, 0x167, 0x17D } },
    { 3, { 0x01B, 0x0C4, 0x174, 0, 0 } },
    { 4, { 0x006, 0x042, 0x17A, 0x179, 0 } },
    { 3, { 0x16F, 0x170, 0x171, 0, 0 } },
    { 4, { 0x104, 0x013, 0, 0x0E6, 0x097 } },
    { 3, { 0x014, 0x0D3, 0x0D6, 0, 0 } },
    { 3, { 0x0EA, 0x090, 0x03B, 0, 0 } },
    { 3, { 0x183, 0x186, 0x189, 0, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 3, { 0x005, 0x00C, 0x0D5, 0, 0 } },
    { 3, { 0, 0x01A, 0x10B, 0x096, 0 } },
    { 3, { 0, 0x182, 0x185, 0x188, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
};
LabRecipe STGDGLAB_recipes2[] = {
    { 3, { 0x183, 0x186, 0x189, 0, 0 } },
    { 5, { 0x103, 0x0FE, 0x094, 0x167, 0x17D } },
    { 3, { 0, 0x01B, 0x0C4, 0x174, 0 } },
    { 1, { 0, 0x038, 0, 0, 0 } },
    { 3, { 0x014, 0x0D3, 0x0D6, 0, 0 } },
    { 3, { 0, 0x0EA, 0x090, 0x03B, 0 } },
    { 3, { 0, 0x176, 0x177, 0x178, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 3, { 0x005, 0x00C, 0x0D5, 0, 0 } },
    { 3, { 0, 0x01A, 0x10B, 0x096, 0 } },
    { 4, { 0, 0x006, 0x042, 0x17A, 0x179 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 3, { 0x182, 0x185, 0x188, 0, 0 } },
    { 4, { 0x104, 0x013, 0, 0x0E6, 0x097 } },
    { 3, { 0, 0x16F, 0x170, 0x171, 0 } },
    { 3, { 0, 0x184, 0x187, 0x18A, 0 } },
};
LabRecipe STGDGLAB_recipes3[] = {
    { 3, { 0x005, 0x00C, 0x0D5, 0, 0 } },
    { 3, { 0, 0x01A, 0x10B, 0x096, 0 } },
    { 3, { 0x16F, 0x170, 0x171, 0, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 3, { 0x183, 0x186, 0x189, 0, 0 } },
    { 3, { 0x176, 0x177, 0x178, 0, 0 } },
    { 5, { 0x103, 0x0FE, 0x094, 0x167, 0x17D } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 3, { 0x01B, 0x0C4, 0x174, 0, 0 } },
    { 3, { 0x182, 0x185, 0x188, 0, 0 } },
    { 4, { 0x104, 0x013, 0, 0x0E6, 0x097 } },
    { 1, { 0, 0x038, 0, 0, 0 } },
    { 3, { 0x184, 0x187, 0x18A, 0, 0 } },
    { 3, { 0x014, 0x0D3, 0x0D6, 0, 0 } },
    { 3, { 0, 0x0EA, 0x090, 0x03B, 0 } },
    { 4, { 0, 0x006, 0x042, 0x17A, 0x179 } },
};
LabRecipe STGDGLAB_recipes4[] = {
    { 5, { 0x103, 0x0FE, 0x094, 0x167, 0x17D } },
    { 4, { 0x104, 0x013, 0, 0x0E6, 0x097 } },
    { 3, { 0, 0x01B, 0x0C4, 0x174, 0 } },
    { 4, { 0, 0x006, 0x042, 0x17A, 0x179 } },
    { 3, { 0x16F, 0x170, 0x171, 0, 0 } },
    { 3, { 0x005, 0x00C, 0x0D5, 0, 0 } },
    { 3, { 0, 0x01A, 0x10B, 0x096, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 3, { 0x176, 0x177, 0x178, 0, 0 } },
    { 3, { 0x014, 0x0D3, 0x0D6, 0, 0 } },
    { 3, { 0, 0x0EA, 0x090, 0x03B, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 1, { 0x038, 0, 0, 0, 0 } },
    { 3, { 0x182, 0x185, 0x188, 0, 0 } },
    { 3, { 0x183, 0x186, 0x189, 0, 0 } },
    { 3, { 0x184, 0x187, 0x18A, 0, 0 } },
};
LabRecipe STGDGLAB_recipes5[] = {
    { 3, { 0x16F, 0x170, 0x171, 0, 0 } },
    { 3, { 0x01B, 0x0C4, 0x174, 0, 0 } },
    { 4, { 0x104, 0x013, 0, 0x0E6, 0x097 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 3, { 0x014, 0x0D3, 0x0D6, 0, 0 } },
    { 1, { 0x038, 0, 0, 0, 0 } },
    { 3, { 0x0EA, 0x090, 0x03B, 0, 0 } },
    { 3, { 0x176, 0x177, 0x178, 0, 0 } },
    { 3, { 0x184, 0x187, 0x18A, 0, 0 } },
    { 5, { 0x103, 0x0FE, 0x094, 0x167, 0x17D } },
    { 3, { 0x005, 0x00C, 0x0D5, 0, 0 } },
    { 3, { 0, 0x01A, 0x10B, 0x096, 0 } },
    { 4, { 0x006, 0x042, 0x17A, 0x179, 0 } },
    { 3, { 0x182, 0x185, 0x188, 0, 0 } },
    { 3, { 0x183, 0x186, 0x189, 0, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
};
LabRecipe STGDGLAB_recipes6[] = {
    { 3, { 0x176, 0x177, 0x178, 0, 0 } },
    { 4, { 0x104, 0x013, 0, 0x0E6, 0x097 } },
    { 3, { 0x0EA, 0x090, 0x03B, 0, 0 } },
    { 3, { 0x16F, 0x170, 0x171, 0, 0 } },
    { 5, { 0x103, 0x0FE, 0x094, 0x167, 0x17D } },
    { 3, { 0x01B, 0x0C4, 0x174, 0, 0 } },
    { 4, { 0x006, 0x042, 0x17A, 0x179, 0 } },
    { 1, { 0x038, 0, 0, 0, 0 } },
    { 3, { 0x014, 0x0D3, 0x0D6, 0, 0 } },
    { 3, { 0x184, 0x187, 0x18A, 0, 0 } },
    { 3, { 0x182, 0x185, 0x188, 0, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 3, { 0x005, 0x00C, 0x0D5, 0, 0 } },
    { 3, { 0, 0x01A, 0x10B, 0x096, 0 } },
    { 3, { 0x183, 0x186, 0x189, 0, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
};
LabRecipe STGDGLAB_recipes7[] = {
    { 3, { 0x014, 0x0D3, 0x0D6, 0, 0 } },
    { 1, { 0x038, 0, 0, 0, 0 } },
    { 3, { 0x0EA, 0x090, 0x03B, 0, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 4, { 0x104, 0x013, 0, 0x0E6, 0x097 } },
    { 3, { 0x005, 0x00C, 0x0D5, 0, 0 } },
    { 3, { 0, 0x01A, 0x10B, 0x096, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 3, { 0x183, 0x186, 0x189, 0, 0 } },
    { 3, { 0, 0x01B, 0x0C4, 0x174, 0 } },
    { 3, { 0, 0x16F, 0x170, 0x171, 0 } },
    { 4, { 0, 0x006, 0x042, 0x17A, 0x179 } },
    { 3, { 0x176, 0x177, 0x178, 0, 0 } },
    { 3, { 0x182, 0x185, 0x188, 0, 0 } },
    { 5, { 0x103, 0x0FE, 0x094, 0x167, 0x17D } },
    { 3, { 0x184, 0x187, 0x18A, 0, 0 } },
};
#elif VERSION_EU || VERSION_JP
LabRecipe STGDGLAB_recipes0[] = {
    { 3, { 0x182, 0x185, 0x188, 0, 0 } },
    { 4, { 0x005, 0x00C, 0x0D5, 0x096, 0 } },
    { 2, { 0, 0x01A, 0x10B, 0, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 5, { 0x103, 0x0FE, 0x094, 0x167, 0x17D } },
    { 3, { 0x01B, 0x0C4, 0x174, 0, 0 } },
    { 4, { 0x104, 0x013, 0, 0x0E6, 0x097 } },
    { 3, { 0x176, 0x177, 0x178, 0, 0 } },
    { 3, { 0x0EA, 0x090, 0x03B, 0, 0 } },
    { 3, { 0x014, 0x0D3, 0x0D6, 0, 0 } },
    { 1, { 0x038, 0, 0, 0, 0 } },
    { 4, { 0x006, 0x042, 0x17A, 0x179, 0 } },
    { 3, { 0x16F, 0x170, 0x171, 0, 0 } },
    { 3, { 0x183, 0x186, 0x189, 0, 0 } },
    { 3, { 0x184, 0x187, 0x18A, 0, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
};
LabRecipe STGDGLAB_recipes1[] = {
    { 3, { 0x184, 0x187, 0x18A, 0, 0 } },
    { 3, { 0x176, 0x177, 0x178, 0, 0 } },
    { 1, { 0x038, 0, 0, 0, 0 } },
    { 5, { 0x103, 0x0FE, 0x094, 0x167, 0x17D } },
    { 3, { 0x01B, 0x0C4, 0x174, 0, 0 } },
    { 4, { 0x006, 0x042, 0x17A, 0x179, 0 } },
    { 3, { 0x16F, 0x170, 0x171, 0, 0 } },
    { 4, { 0x104, 0x013, 0, 0x0E6, 0x097 } },
    { 3, { 0x014, 0x0D3, 0x0D6, 0, 0 } },
    { 3, { 0x0EA, 0x090, 0x03B, 0, 0 } },
    { 3, { 0x183, 0x186, 0x189, 0, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 4, { 0x005, 0x00C, 0x0D5, 0x096, 0 } },
    { 2, { 0, 0x01A, 0x10B, 0, 0 } },
    { 3, { 0, 0x182, 0x185, 0x188, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
};
LabRecipe STGDGLAB_recipes2[] = {
    { 3, { 0x183, 0x186, 0x189, 0, 0 } },
    { 5, { 0x103, 0x0FE, 0x094, 0x167, 0x17D } },
    { 3, { 0, 0x01B, 0x0C4, 0x174, 0 } },
    { 1, { 0, 0x038, 0, 0, 0 } },
    { 3, { 0x014, 0x0D3, 0x0D6, 0, 0 } },
    { 3, { 0, 0x0EA, 0x090, 0x03B, 0 } },
    { 3, { 0, 0x176, 0x177, 0x178, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 4, { 0x005, 0x00C, 0x0D5, 0x096, 0 } },
    { 2, { 0, 0x01A, 0x10B, 0, 0 } },
    { 4, { 0, 0x006, 0x042, 0x17A, 0x179 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 3, { 0x182, 0x185, 0x188, 0, 0 } },
    { 4, { 0x104, 0x013, 0, 0x0E6, 0x097 } },
    { 3, { 0, 0x16F, 0x170, 0x171, 0 } },
    { 3, { 0, 0x184, 0x187, 0x18A, 0 } },
};
LabRecipe STGDGLAB_recipes3[] = {
    { 4, { 0x005, 0x00C, 0x0D5, 0x096, 0 } },
    { 2, { 0, 0x01A, 0x10B, 0, 0 } },
    { 3, { 0x16F, 0x170, 0x171, 0, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 3, { 0x183, 0x186, 0x189, 0, 0 } },
    { 3, { 0x176, 0x177, 0x178, 0, 0 } },
    { 5, { 0x103, 0x0FE, 0x094, 0x167, 0x17D } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 3, { 0x01B, 0x0C4, 0x174, 0, 0 } },
    { 3, { 0x182, 0x185, 0x188, 0, 0 } },
    { 4, { 0x104, 0x013, 0, 0x0E6, 0x097 } },
    { 1, { 0, 0x038, 0, 0, 0 } },
    { 3, { 0x184, 0x187, 0x18A, 0, 0 } },
    { 3, { 0x014, 0x0D3, 0x0D6, 0, 0 } },
    { 3, { 0, 0x0EA, 0x090, 0x03B, 0 } },
    { 4, { 0, 0x006, 0x042, 0x17A, 0x179 } },
};
LabRecipe STGDGLAB_recipes4[] = {
    { 5, { 0x103, 0x0FE, 0x094, 0x167, 0x17D } },
    { 4, { 0x104, 0x013, 0, 0x0E6, 0x097 } },
    { 3, { 0, 0x01B, 0x0C4, 0x174, 0 } },
    { 4, { 0, 0x006, 0x042, 0x17A, 0x179 } },
    { 3, { 0x16F, 0x170, 0x171, 0, 0 } },
    { 4, { 0x005, 0x00C, 0x0D5, 0x096, 0 } },
    { 2, { 0, 0x01A, 0x10B, 0, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 3, { 0x176, 0x177, 0x178, 0, 0 } },
    { 3, { 0x014, 0x0D3, 0x0D6, 0, 0 } },
    { 3, { 0, 0x0EA, 0x090, 0x03B, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 1, { 0x038, 0, 0, 0, 0 } },
    { 3, { 0x182, 0x185, 0x188, 0, 0 } },
    { 3, { 0x183, 0x186, 0x189, 0, 0 } },
    { 3, { 0x184, 0x187, 0x18A, 0, 0 } },
};
LabRecipe STGDGLAB_recipes5[] = {
    { 3, { 0x16F, 0x170, 0x171, 0, 0 } },
    { 3, { 0x01B, 0x0C4, 0x174, 0, 0 } },
    { 4, { 0x104, 0x013, 0, 0x0E6, 0x097 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 3, { 0x014, 0x0D3, 0x0D6, 0, 0 } },
    { 1, { 0x038, 0, 0, 0, 0 } },
    { 3, { 0x0EA, 0x090, 0x03B, 0, 0 } },
    { 3, { 0x176, 0x177, 0x178, 0, 0 } },
    { 3, { 0x184, 0x187, 0x18A, 0, 0 } },
    { 5, { 0x103, 0x0FE, 0x094, 0x167, 0x17D } },
    { 4, { 0x005, 0x00C, 0x0D5, 0x096, 0 } },
    { 2, { 0, 0x01A, 0x10B, 0, 0 } },
    { 4, { 0x006, 0x042, 0x17A, 0x179, 0 } },
    { 3, { 0x182, 0x185, 0x188, 0, 0 } },
    { 3, { 0x183, 0x186, 0x189, 0, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
};
LabRecipe STGDGLAB_recipes6[] = {
    { 3, { 0x176, 0x177, 0x178, 0, 0 } },
    { 4, { 0x104, 0x013, 0, 0x0E6, 0x097 } },
    { 3, { 0x0EA, 0x090, 0x03B, 0, 0 } },
    { 3, { 0x16F, 0x170, 0x171, 0, 0 } },
    { 5, { 0x103, 0x0FE, 0x094, 0x167, 0x17D } },
    { 3, { 0x01B, 0x0C4, 0x174, 0, 0 } },
    { 4, { 0x006, 0x042, 0x17A, 0x179, 0 } },
    { 1, { 0x038, 0, 0, 0, 0 } },
    { 3, { 0x014, 0x0D3, 0x0D6, 0, 0 } },
    { 3, { 0x184, 0x187, 0x18A, 0, 0 } },
    { 3, { 0x182, 0x185, 0x188, 0, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 4, { 0x005, 0x00C, 0x0D5, 0x096, 0 } },
    { 2, { 0, 0x01A, 0x10B, 0, 0 } },
    { 3, { 0x183, 0x186, 0x189, 0, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
};
LabRecipe STGDGLAB_recipes7[] = {
    { 3, { 0x014, 0x0D3, 0x0D6, 0, 0 } },
    { 1, { 0x038, 0, 0, 0, 0 } },
    { 3, { 0x0EA, 0x090, 0x03B, 0, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 4, { 0x104, 0x013, 0, 0x0E6, 0x097 } },
    { 4, { 0x005, 0x00C, 0x0D5, 0x096, 0 } },
    { 2, { 0, 0x01A, 0x10B, 0, 0 } },
    { 0, { 0, 0, 0, 0, 0 } },
    { 3, { 0x183, 0x186, 0x189, 0, 0 } },
    { 3, { 0, 0x01B, 0x0C4, 0x174, 0 } },
    { 3, { 0, 0x16F, 0x170, 0x171, 0 } },
    { 4, { 0, 0x006, 0x042, 0x17A, 0x179 } },
    { 3, { 0x176, 0x177, 0x178, 0, 0 } },
    { 3, { 0x182, 0x185, 0x188, 0, 0 } },
    { 5, { 0x103, 0x0FE, 0x094, 0x167, 0x17D } },
    { 3, { 0x184, 0x187, 0x18A, 0, 0 } },
};
#endif
LabData STGDGLAB_data = {
    STGDGLAB_partnerAnims,
    STGDGLAB_layout,
    {
        STGDGLAB_recipes0, STGDGLAB_recipes1, STGDGLAB_recipes2, STGDGLAB_recipes3,
        STGDGLAB_recipes4, STGDGLAB_recipes5, STGDGLAB_recipes6, STGDGLAB_recipes7,
    },
    {
        STGDGLAB_loadFiles, STGDGLAB_filesLoading, STGDGLAB_startFade, STGDGLAB_updateFade,
        STGDGLAB_startLerp, STGDGLAB_updateLerp, STGDGLAB_getItemSprite, STGDGLAB_getItemB,
    },
};
