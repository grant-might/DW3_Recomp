/* The shop's main task, with its menu and the money; the files it loads; and
   the panels' openings and moves */

#include "menus/item_shop.h"

/* Creates the shop's windows: the help, the shop's name, the money, buy and sell,
   and the cursor */
void STITSHOP_createShopWindows(ItemShop *shop, ItemShopWindows *win) {
    win->help = createTextWindow(shop->layer, 1, 0xD3, 0xCC);
    win->title = createTextWindow(shop->layer, 1, 0x1D, 0x14);
    win->money = createTextWindow(shop->layer, 3, 0x117, 0x17);
    win->moneyLabel = createTextWindow(shop->layer, 3, 0x11A, 0x17);
    win->buy = createTextWindow(shop->layer, 1, 0xBE, 0x2F);
    win->sell = createTextWindow(shop->layer, 1, 0xBE, 0x3D);
    win->cursor = createCursor(shop->layer, 0, 0xB0, 0x2F);
    win->cursor->setVisible(win->cursor, 0);
}

/* Draws the shop's four frames as they open by scaling, and the background,
   which scrolls one pixel every other frame, wrapping at 96 */
void STITSHOP_drawShop(ItemShop *shop, ItemShopWindows *win) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(shop->layer, 1);
    sprite.setTexture(0x280, 0x100);
    if (shop->panels[0].level != 0) {
        if (shop->panels[0].level != ONE) {
            sprite.setScale(shop->panels[0].level, ONE, ONE);
            sprite.setPivot(0x57, 0x19);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 1, 0x16, 0x12);
    }
    if (shop->panels[1].level != 0) {
        if (shop->panels[1].level != ONE) {
            sprite.setScale(shop->panels[1].level, ONE, ONE);
            sprite.setPivot(0x140, 0x18);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 2, 0xD6, 0xF);
    }
    if (shop->panels[2].level != 0) {
        if (shop->panels[2].level != ONE) {
            sprite.setScale(shop->panels[2].level, ONE, ONE);
            sprite.setPivot(0x140, 0x3B);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 5, 0xA9, 0x26);
    }
    if (shop->panels[3].level != 0) {
        if (shop->panels[3].level != ONE) {
            sprite.setScale(shop->panels[3].level, ONE, ONE);
            sprite.setPivot(0x140, 0xD2);
        } else {
            sprite.setScale(ONE, ONE, ONE);
        }
        sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0, 0xC6, 0xC4);
    }
    sprite.setLayerId(shop->layer, 7);
    if (shop->bgTick != 0) {
        shop->bgScroll++;
        shop->bgScroll = shop->bgScroll < 0x60 ? shop->bgScroll : 0;
        shop->bgTick = 0;
    } else {
        shop->bgTick = 1;
    }
    sprite.setScale(ONE, ONE, ONE);
    sprite.draw(FILE_CACHE.getEntry(FILE_SHOP_SPRITES << 16), 0x14, shop->bgScroll, shop->bgScroll);
}

/* Opens the shop's panels, then up/down and cross pick buy or sell and run that
   dialog, reopening after it; triangle closes the panels and fades out */
void STITSHOP_runShop(ItemShop *shop, ItemShopWindows *win) {
    s32 choice;

    switch (shop->substate) {
    case 0:
    default:
        STITSHOP_funcs.startFade(&shop->panels[0], 1);
        STITSHOP_funcs.startFade(&shop->panels[1], 1);
        shop->substate++;
        break;
    case 1:
        STITSHOP_funcs.updateFade(&shop->panels[0]);
        if (STITSHOP_funcs.updateFade(&shop->panels[1]) != 0) {
            STITSHOP_funcs.startFade(&shop->panels[2], 1);
            STITSHOP_funcs.startFade(&shop->panels[3], 1);
            win->title->setString(win->title, FILE_CACHE.load(TEXT_FILE(TEXT_SHOP_NAMES)), STITSHOP_shopNames[shop->shop]);
            win->moneyLabel->setString(win->moneyLabel, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 2);
            win->money->setNumber(win->money, 0, GAME.money);
            win->money->setRightAlign(win->money, 1);
            shop->substate++;
        }
        break;
    case 2:
        STITSHOP_funcs.updateFade(&shop->panels[2]);
        if (STITSHOP_funcs.updateFade(&shop->panels[3]) != 0) {
            win->buy->setString(win->buy, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 3);
            win->sell->setString(win->sell, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 4);
            win->cursor->setVisible(win->cursor, 1);
            win->help->setString(win->help, FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_SHOP)), 5);
            shop->substate++;
        }
        break;
    case 3:
        choice = shop->choice;
        if (PAD_PRESSED(PAD_UP)) {
            shop->choice = 0;
        } else if (PAD_PRESSED(PAD_DOWN)) {
            shop->choice = 1;
        }
        if (choice != shop->choice) {
            SOUND.playSound(SOUND_CURSOR);
            win->cursor->setPos(win->cursor, 0xB0, shop->choice * 0xE + 0x2F);
        }
        if (PAD_PRESSED(PAD_CROSS)) {
            SOUND.playSound(SOUND_SELECT);
            shop->substate = 20;
            shop->step = 1;
        } else if (PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CANCEL);
            shop->setSubstate(shop, 20);
        }
        break;
    case 10:
        if (shop->choice == 0) {
            win->dialog = (Task *)STITSHOP_createBuy(shop);
        } else {
            win->dialog = (Task *)STITSHOP_createSell(shop);
        }
        shop->substate++;
        break;
    case 11:
        if (win->dialog == NULL) {
            STITSHOP_funcs.startFade(&shop->panels[2], 1);
            STITSHOP_funcs.startFade(&shop->panels[3], 1);
            shop->substate = 2;
        }
        break;
    case 20:
        if (shop->step == 0) {
            win->fade = STITSHOP_createFader();
            win->fade->start(win->fade, 0, 0x1E);
        }
        STITSHOP_funcs.startFade(&shop->panels[2], 0);
        STITSHOP_funcs.startFade(&shop->panels[3], 0);
        win->help->setVisible(win->help, 0);
        win->buy->setVisible(win->buy, 0);
        win->sell->setVisible(win->sell, 0);
        win->cursor->setVisible(win->cursor, 0);
        shop->substate++;
        break;
    case 21:
        STITSHOP_funcs.updateFade(&shop->panels[2]);
        if (STITSHOP_funcs.updateFade(&shop->panels[3]) != 0) {
            if (shop->step != 0) {
                shop->setSubstate(shop, 10);
            } else {
                STITSHOP_funcs.startFade(&shop->panels[0], 0);
                STITSHOP_funcs.startFade(&shop->panels[1], 0);
                win->title->setVisible(win->title, 0);
                win->moneyLabel->setVisible(win->moneyLabel, 0);
                win->money->setVisible(win->money, 0);
                shop->substate++;
            }
        }
        break;
    case 22:
        STITSHOP_funcs.updateFade(&shop->panels[0]);
        if (STITSHOP_funcs.updateFade(&shop->panels[1]) != 0) {
            shop->substate++;
        }
        break;
    case 23:
        if (win->fade->state == TASK_DONE) {
            shop->state = TASK_KILL;
        }
        break;
    }
}

/* The shop's task: loads its files, then runs it; goes back to the field when
   killed */
void STITSHOP_updateShop(ItemShop *shop, ItemShopWindows *win) {
    switch (shop->state) {
    case TASK_INIT:
    default:
        switch (shop->substate) {
        case 0:
        default:
            STITSHOP_funcs.loadFiles();
            shop->substate++;
            break;
        case 1:
            if (STITSHOP_funcs.filesLoading() == 0) {
                STITSHOP_createShopWindows(shop, win);
                shop->panels[0].duration = 10;
                shop->panels[1].duration = 10;
                shop->panels[2].duration = 10;
                shop->panels[3].duration = 10;
                shop->nextState(shop);
            }
            break;
        }
        break;
    case TASK_RUN:
        STITSHOP_runShop(shop, win);
        STITSHOP_drawShop(shop, win);
        break;
    case TASK_DONE:
        break;
    case TASK_KILL:
        GAME.funcs.requestMode(GAME.fieldMode, 0);
        break;
    }
}

/* Shows the money left (shop->showMoney), after buying or selling */
void STITSHOP_showMoney(ItemShop *shop) {
    ItemShopWindows *win = shop->children;

    win->money->setNumber(win->money, 0, GAME.money);
    win->money->setRightAlign(win->money, 1);
}

/* Creates the item shop (task) for the shop of the mode's argument */
ItemShop *STITSHOP_createShop(void) {
    ItemShop *shop = createTask(STITSHOP_updateShop, sizeof(ItemShop), sizeof(ItemShopWindows));

    shop->showMoney = STITSHOP_showMoney;
    shop->layer = SCREEN_LAYER;
    shop->shop = GAME.funcs.getModeArg();
    return shop;
}

/* Loads the shop's textures and requests its strings */
void STITSHOP_loadFiles(void) {
    TimLoader loader;

    initTimLoader(&loader);
    loader.setImagePos(0x280, 0x100);
    loader.loadArchive(FILE_CACHE.getEntry((FILE_SHOP_SPRITES + 1) << 16));
    FILE_CACHE.request(TEXT_FILE(TEXT_ITEM_SHOP));
    FILE_CACHE.request(TEXT_FILE(TEXT_ITEM_NAMES));
    FILE_CACHE.request(TEXT_FILE(TEXT_ITEM_INFO));
    FILE_CACHE.request(TEXT_FILE(TEXT_SHOP_NAMES));
}

/* Whether the shop's strings are still loading */
s32 STITSHOP_filesLoading(void) {
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_ITEM_SHOP)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_ITEM_NAMES)) != 0) {
        return 1;
    }
    if (FILE_CACHE.isLoading(TEXT_FILE(TEXT_ITEM_INFO)) != 0) {
        return 1;
    }
    return FILE_CACHE.isLoading(TEXT_FILE(TEXT_SHOP_NAMES)) != 0;
}

#include "shared/start_fade.inc.c"
#include "shared/update_fade.inc.c"
#include "shared/start_lerp.inc.c"
#include "shared/update_lerp.inc.c"
