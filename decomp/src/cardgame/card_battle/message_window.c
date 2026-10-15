/* CARDGAME's message and menu windows, and the hand's spacing. */

#include "cardgame/card_battle.h"

/* Draws the message window's frame at a scale */
void CARDGAME_drawMessageFrame(CardScreen *screen, CardScreenItems *items, s16 scale, s32 x, s32 y) {
    SpriteDrawer drawer;

    initSpriteDrawer(&drawer);
    drawer.setLayerId(0x100, 1);
    drawer.setTexture(0x280, 0);
    drawer.setPivot(0, 0x78);
    drawer.setScale(scale, 0x1000, 0x1000);
    drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 2), 0x1A, x, y);
}

/* Draws sprite 0x18 of the message window */
void CARDGAME_drawMessageMark(CardScreen *screen, CardScreenItems *items, s32 x, s32 y) {
    SpriteDrawer drawer;

    initSpriteDrawer(&drawer);
    drawer.setLayerId(0x100, 1);
    drawer.setTexture(0x280, 0);
    drawer.setScale(0x1000, 0x1000, 0x1000);
    drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 2), 0x18, x, y);
}

/* Shows the message window's prompt and cursor, or hides them when it has
   no prompt */
static inline void showMessagePrompt(CardScreen *screen, CardScreenItems *items) {
#if VERSION_US
    if (screen->message.prompt != 0) {
        items->cursor->setVisible(items->cursor, 1);
        items->cursor->setPos(items->cursor, 0x14, CARDGAME_messageWindowPositions[screen->message.place][1] + 0x11 + screen->message.choice * 14);
        items->moreTexts[7]->setString(items->moreTexts[7], FILE_CACHE.load(TEXT_FILE(TEXT_CARD_GAME)), 0x18);
        items->moreTexts[7]->setPos(items->moreTexts[7], CARDGAME_messageWindowPositions[screen->message.place][0] + 0x1B, CARDGAME_messageWindowPositions[screen->message.place][1] + 0x12);
    } else {
        items->moreTexts[7]->setVisible(items->moreTexts[7], 0);
        items->cursor->setVisible(items->cursor, 0);
    }
#elif VERSION_EU || VERSION_JP
    switch (screen->message.prompt) {
    case 0:
    default:
        items->moreTexts[7]->setVisible(items->moreTexts[7], 0);
        items->cursor->setVisible(items->cursor, 0);
        break;
    case 1:
        items->cursor->setVisible(items->cursor, 1);
        items->cursor->setPos(items->cursor, 0x14, CARDGAME_messageWindowPositions[screen->message.place][1] + 0x11 + screen->message.choice * 14);
        items->moreTexts[7]->setString(items->moreTexts[7], FILE_CACHE.load(TEXT_FILE(TEXT_CARD_GAME)), 0x18);
        items->moreTexts[7]->setPos(items->moreTexts[7], CARDGAME_messageWindowPositions[screen->message.place][0] + 0x1B, CARDGAME_messageWindowPositions[screen->message.place][1] + 0x12);
        break;
    case 2:
    case 3:
        items->cursor->setVisible(items->cursor, 1);
        items->cursor->setPos(items->cursor, 0x14, CARDGAME_messageWindowPositions[screen->message.place][1] + 0x11 + screen->message.choice * 14);
        items->moreTexts[7]->setString(items->moreTexts[7], FILE_CACHE.load(TEXT_FILE(TEXT_CARD_GAME)), 0x45);
        if (screen->message.prompt == 3) {
            items->moreTexts[7]->setPalette(items->moreTexts[7], PALETTE_GREY);
        }
        items->moreTexts[7]->setPos(items->moreTexts[7], CARDGAME_messageWindowPositions[screen->message.place][0] + 0x1B, CARDGAME_messageWindowPositions[screen->message.place][1] + 0x12);
        items->texts[0]->setString(items->texts[0], FILE_CACHE.load(TEXT_FILE(TEXT_CARD_GAME)), 0x46);
        items->texts[0]->setRightAlign(items->texts[0], 0);
        items->texts[0]->setPos(items->texts[0], CARDGAME_messageWindowPositions[screen->message.place][0] + 0x1B, CARDGAME_messageWindowPositions[screen->message.place][1] + 0x20);
        break;
    }
#endif
}

/* Shows the message window's text: an item's name, the message with its wins
   or opponent numbers, or nothing */
static inline void showMessageText(CardScreen *screen, CardScreenItems *items) {
    TextTools tools;
    s32 i;
    s32 x;

    if (screen->message.place == 3) {
        items->moreTexts[1]->setPos(items->moreTexts[1], CARDGAME_messageWindowPositions[3][0] + 0x40, CARDGAME_messageWindowPositions[3][1] + 4);
        items->moreTexts[1]->setString(items->moreTexts[1], FILE_CACHE.load(TEXT_FILE(TEXT_CARD_GAME)), 0x42);
        items->moreTexts[7]->setString(items->moreTexts[7], FILE_CACHE.load(TEXT_FILE(TEXT_ITEM_NAMES)), screen->message.message);
        items->moreTexts[7]->setPos(items->moreTexts[7], CARDGAME_messageWindowPositions[screen->message.place][0] + 0x40, CARDGAME_messageWindowPositions[screen->message.place][1] + 0x12);
    } else if (screen->message.message != 0) {
        items->moreTexts[1]->setPos(items->moreTexts[1], CARDGAME_messageWindowPositions[screen->message.place][0] + 0x1B, CARDGAME_messageWindowPositions[screen->message.place][1] + 4);
        items->moreTexts[1]->setString(items->moreTexts[1], FILE_CACHE.load(TEXT_FILE(TEXT_CARD_GAME)), screen->message.message);
        if (screen->message.message == 0x13) {
            items->moreTexts[7]->setString(items->moreTexts[7], FILE_CACHE.load(TEXT_FILE(TEXT_CARD_GAME)), 0x2B);
            items->moreTexts[7]->setPos(items->moreTexts[7], CARDGAME_messageWindowPositions[screen->message.place][0] + 0x1B, CARDGAME_messageWindowPositions[screen->message.place][1] + 0x12);
            initTextTools(&tools);
            x = tools.measure(items->moreTexts[7]->text, items->moreTexts[7]->style, items->moreTexts[7]->spacingX) + 0x1B;
            for (i = 0; i < 2; i++) {
                items->texts[i]->setPos(items->texts[i], CARDGAME_messageWindowPositions[screen->message.place][0] + x + 0xE, CARDGAME_messageWindowPositions[screen->message.place][1] + 0x12 + i * 14);
                items->texts[i]->setNumber(items->texts[i], 0, screen->panels[i].wins);
                items->texts[i]->setRightAlign(items->texts[i], 0);
            }
        } else if (screen->message.message == 0x3E) {
            items->texts[0]->setPos(items->texts[0], CARDGAME_messageWindowPositions[screen->message.place][0] + 0x3E, CARDGAME_messageWindowPositions[screen->message.place][1] + 4);
            items->texts[0]->setNumber(items->texts[0], 0, screen->opponentLevel);
            items->texts[0]->setRightAlign(items->texts[0], 1);
            items->texts[1]->setString(items->texts[1], FILE_CACHE.load(TEXT_FILE(TEXT_DECK_EDITOR)), (screen->opponent + 1) / 2);
            items->texts[1]->setPos(items->texts[1], CARDGAME_messageWindowPositions[screen->message.place][0] + 0x1B, CARDGAME_messageWindowPositions[screen->message.place][1] + 0x12);
        }
    } else {
        items->moreTexts[1]->setVisible(items->moreTexts[1], 0);
    }
}

/* The message window: it opens at its place, shows its prompt
   (showMessagePrompt) and message (showMessageText) with the cursor, then closes */
void CARDGAME_updateMessageWindow(CardScreen *screen, CardScreenItems *items) {
    s16 scale;
    s32 t;

    if (screen->message.state == 0) {
        return;
    }
    switch (screen->message.state) {
    case 1:
    default:
        t = screen->message.time << 12;
        scale = 0x1000 - (screen->message.duration != 0 ? t / screen->message.duration : t);
        screen->message.time -= GFX.funcs.getFrameTime();
        if (screen->message.time <= 0) {
            screen->message.state = 2;
            showMessagePrompt(screen, items);
            showMessageText(screen, items);
        }
        break;
    case 2:
        scale = 0x1000;
        if (screen->message.place == 3) {
            CARDGAME_drawMessageMark(screen, items, 0x1C, 0x64);
        }
        if (screen->message.prompt != 0) {
            items->cursor->setPos(items->cursor, 0x14, CARDGAME_messageWindowPositions[screen->message.place][1] + 0x11 + screen->message.choice * 14);
        }
        break;
    case 4:
        scale = 0x1000;
        items->cursor->setPos(items->cursor, 0x14 - rsin((screen->message.time << 12) / 20) / 512, CARDGAME_messageWindowPositions[screen->message.place][1] + 0x11 + screen->message.choice * 14);
        screen->message.time -= GFX.funcs.getFrameTime();
        if (screen->message.time <= 0) {
            items->cursor->setVisible(items->cursor, 0);
            screen->closeMessage(screen);
            items->cursor->setIdleDelay(items->cursor, 0x20);
            screen->message.state = 5;
        }
        break;
    case 5:
        items->cursor->setVisible(items->cursor, 0);
        items->moreTexts[1]->setVisible(items->moreTexts[1], 0);
#if VERSION_EU || VERSION_JP
        items->moreTexts[7]->setPalette(items->moreTexts[7], PALETTE_WHITE);
#endif
        items->moreTexts[7]->setVisible(items->moreTexts[7], 0);
        items->texts[0]->setVisible(items->texts[0], 0);
        items->texts[1]->setVisible(items->texts[1], 0);
        items->texts[2]->setVisible(items->texts[2], 0);
        screen->message.time -= GFX.funcs.getFrameTime();
        if (screen->message.time <= 0) {
            screen->message.state = 0;
        }
        scale = (screen->message.time << 12) / screen->message.duration;
        break;
    }
    CARDGAME_drawMessageFrame(screen, items, scale, CARDGAME_messageWindowPositions[screen->message.place][0], CARDGAME_messageWindowPositions[screen->message.place][1]);
}

/* Draws the menu window's frame at a scale */
void CARDGAME_drawMenuFrame(CardScreen *screen, CardScreenItems *items, s16 scale, s32 x, s32 y) {
    SpriteDrawer drawer;

    initSpriteDrawer(&drawer);
    drawer.setLayerId(0x100, 1);
    drawer.setTexture(0x340, 0);
    drawer.setPivot(0, 0x86);
    drawer.setScale(scale, 0x1000, 0x1000);
    drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), 1, x, y);
}

/* A menu window (CardScreen.menuState): it opens, shows text file 0x10 with
   the cursor on menuRow, then closes */
void CARDGAME_updateMenuWindow(CardScreen *screen, CardScreenItems *items) {
    s16 scale;
    s32 t;

    if (screen->menuState == 0) {
        return;
    }
    switch (screen->menuState) {
    case 1:
    default:
        t = screen->menuTime << 12;
        scale = 0x1000 - (screen->menuDuration != 0 ? t / screen->menuDuration : t);
        screen->menuTime -= GFX.funcs.getFrameTime();
        if (screen->menuTime <= 0) {
            screen->menuState = 2;
            items->cursor->setVisible(items->cursor, 1);
            items->cursor->setPos(items->cursor, 10, screen->menuRow * 14 + 0x65);
            items->moreTexts[6]->setString(items->moreTexts[6], FILE_CACHE.load(TEXT_FILE(TEXT_CARD_GAME)), 0x20);
            items->moreTexts[6]->setPos(items->moreTexts[6], 0x18, 0x65);
        }
        break;
    case 2:
        scale = 0x1000;
        items->cursor->setPos(items->cursor, 10, screen->menuRow * 14 + 0x65);
        break;
    case 4:
        scale = 0x1000;
        items->cursor->setPos(items->cursor, 10 - rsin((screen->menuTime << 12) / 20) / 512, screen->menuRow * 14 + 0x65);
        screen->menuTime -= GFX.funcs.getFrameTime();
        if (screen->menuTime <= 0) {
            items->cursor->setVisible(items->cursor, 0);
            screen->closeMenu(screen);
            items->cursor->setIdleDelay(items->cursor, 0x20);
            screen->menuState = 5;
        }
        break;
    case 5:
        items->cursor->setVisible(items->cursor, 0);
        items->moreTexts[6]->setVisible(items->moreTexts[6], 0);
        screen->menuTime -= GFX.funcs.getFrameTime();
        if (screen->menuTime <= 0) {
            screen->menuState = 0;
        }
        scale = (screen->menuTime << 12) / screen->menuDuration;
        break;
    }
    CARDGAME_drawMenuFrame(screen, items, scale, 0, 0x60);
}

/* CardScreen.getHandOffset: how far the hand's card index is from its first:
   0x2900 a card up to six cards, else count share 0xF600 */
s32 CARDGAME_getHandOffset(s32 count, s32 index) {
    s32 step;

    if (count < 7) {
        step = 0x2900;
    } else {
        step = 0xF600 / count;
    }
    return step * index;
}
