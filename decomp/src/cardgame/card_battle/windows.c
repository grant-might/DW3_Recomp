/* CARDGAME's screen background, numbers, gauges and windows. */

#include "cardgame/card_battle.h"

/* Draws the scrolling background, which fades in through its palettes (fadeState
   1) and then stays */
void CARDGAME_drawBackground(CardScreen *screen) {
    SpriteDrawer drawer;
    s32 row;
    s32 x;

    initSpriteDrawer(&drawer);
    drawer.setLayerId(0x100, 2);
    row = 0;
    drawer.setTexture(0x340, 0);
    switch (screen->fadeState) {
    case 1:
        if (screen->fadeTime >= 4) {
            screen->fadeTime -= 4;
            if (++screen->fadeRow >= 11) {
                screen->fadeRow = 11;
                screen->fadeState = 2;
            }
        }
        row = screen->fadeRow;
        screen->fadeTime += GFX.funcs.getFrameTime();
        break;
    case 2:
        row = 11;
        break;
    case 0:
        break;
    }
    drawer.setClutRow(row);
    x = (screen->time >> 1) & 0x3F;
    drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), 0, x, x);
}

/* Draws a number with the TIM archive's digits, seven pixels apart, without
   leading zeros unless asked; scaled about its pivot when scaled */
void CARDGAME_drawNumber(CardNumber *number, s32 scaled) {
    SpriteDrawer drawer;
    s32 value;
    s32 digit;
    s32 i;

    initSpriteDrawer(&drawer);
    drawer.setLayerId(0x100, number->depth);
    drawer.setTexture(0x340, 0);
    if (scaled != 0) {
        drawer.setPivot(number->pivotX, number->pivotY);
        drawer.setScale(number->scaleX, number->scaleY, 0x1000);
    }
    value = number->value;
    for (i = 0; i < number->digits; i++) {
        digit = value % 10;
        if (i == 0 || number->leadingZeros != 0 || digit != 0 || value / 10 != 0) {
            drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), digit + 0x14,
                        number->x + (number->digits - 1 - i) * 7, number->y);
        }
        value /= 10;
    }
}

/* Draws a gauge at its place (CARDGAME_gaugePositions), at its scale */
void CARDGAME_drawGauge(CardScreen *screen, CardScreenItems *items, s32 index, CardGauge *p) {
    SpriteDrawer drawer;

    initSpriteDrawer(&drawer);
    drawer.setLayerId(0x100, 1);
    drawer.setTexture(0x340, 0);
    drawer.setPivot(CARDGAME_gaugePositions[index].x + 4, CARDGAME_gaugePositions[index].y + 23);
    drawer.setScale(0x1000, p->to, 0x1000);
    drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), p->value + 6, CARDGAME_gaugePositions[index].x, CARDGAME_gaugePositions[index].y);
}

/* Grows (state 1), holds (2) or shrinks (3) a gauge, and draws it */
void CARDGAME_updateGauge(CardScreen *screen, CardScreenItems *items, s32 index, CardGauge *p) {
    if (p->state != 0) {
        switch (p->state) {
        case 1:
        default:
            p->to = 0x1000 - (p->time << 12) / p->duration;
            p->time -= GFX.funcs.getFrameTime();
            if (p->time <= 0) {
                p->state = 2;
            }
            break;
        case 2:
            p->to = 0x1000;
            break;
        case 3:
            p->time -= GFX.funcs.getFrameTime();
            if (p->time <= 0) {
                p->state = 0;
            }
            p->to = (p->time << 12) / p->duration;
            break;
        }
        CARDGAME_drawGauge(screen, items, index, p);
    }
}

/* Runs and draws the three gauges */
void CARDGAME_updateGauges(CardScreen *screen, CardScreenItems *items) {
    s32 i;

    for (i = 0; i < 3; i++) {
        CARDGAME_updateGauge(screen, items, i, &screen->gauges[i]);
    }
}

/* Draws the number in a type 3 window: its low four bits, and the sprite
   for its high bits */
static inline void drawWindowCount(CardWindow *window) {
    CardNumber number;
    SpriteDrawer digits;

    number.depth = 1;
    number.digits = 2;
    number.leadingZeros = 0;
    number.x = window->x + 0x18;
    number.y = window->y + 4;
    number.value = window->value & 0xF;
    number.pivotX = window->x + CARDGAME_windowLayouts[window->layout].x;
    number.pivotY = window->y + CARDGAME_windowLayouts[window->layout].y;
    number.scaleX = window->from;
    number.scaleY = 0x1000;
    CARDGAME_drawNumber(&number, 1);
    initSpriteDrawer(&digits);
    digits.setLayerId(0x100, 1);
    digits.setTexture(0x280, 0);
    digits.setPivot(window->x + CARDGAME_windowLayouts[window->layout].x,
                    window->y + CARDGAME_windowLayouts[window->layout].y);
    digits.setScale(window->from, 0x1000, 0x1000);
    digits.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 2), (window->value >> 4) * 3 + 0x1D,
                window->x + 6, window->y + 2);
}

/* Draws a window's frame (CARDGAME_windowLayouts), scaled while it opens, and
   its count */
void CARDGAME_drawWindowFrame(CardScreen *screen, CardScreenItems *items, CardWindow *window) {
    s32 offset = 0;

    if (window->layout == 2 && window->numbers[2] == 1) {
        offset = 0x45;
    }
    if (window->state == 2 && window->layout == 3 && window->showCount != 0) {
        drawWindowCount(window);
    }
    {
    SpriteDrawer frame;

    if (CARDGAME_windowLayouts[window->layout].kind == 2) {
        initSpriteDrawer(&frame);
        frame.setLayerId(0x100, 1);
        frame.setTexture(0x340, 0);
        if (window->from != 0x1000) {
            frame.setPivot(window->x + CARDGAME_windowLayouts[window->layout].x,
                           window->y + CARDGAME_windowLayouts[window->layout].y);
            frame.setScale(window->from, 0x1000, 0x1000);
        }
        frame.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), CARDGAME_windowLayouts[window->layout].sprite,
                   window->x + offset, window->y);
    } else {
        initSpriteDrawer(&frame);
        frame.setLayerId(0x100, 1);
        frame.setTexture(0x280, 0);
        if (window->from != 0x1000) {
            frame.setPivot(window->x + CARDGAME_windowLayouts[window->layout].x,
                           window->y + CARDGAME_windowLayouts[window->layout].y);
            frame.setScale(window->from, 0x1000, 0x1000);
        }
        frame.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 2), CARDGAME_windowLayouts[window->layout].sprite,
                   window->x + offset, window->y);
    }
    }
}

/* Puts a window's two numbers (numbers) on its texts, right aligned */
void CARDGAME_drawWindowNumbers(CardScreen *screen, CardScreenItems *items, CardWindow *window) {
    s32 values[2];
    s32 i;

    values[0] = window->numbers[0];
    values[1] = window->numbers[1];
    for (i = 0; i < 2; i++) {
        items->texts[i]->setPos(items->texts[i], window->x + 0x67, window->y + 4 + i * 13);
        items->texts[i]->setNumber(items->texts[i], 0, values[i]);
        items->texts[i]->setRightAlign(items->texts[i], 1);
    }
}

/* Puts a window's string value of file on text, or hides text when it has
   none */
void CARDGAME_drawWindowText(CardScreen *screen, CardWindow *window, TextWindow *text, s32 file, s32 index) {
    if (window->value != 0) {
        text->setPos(text, window->x + CARDGAME_windowTextOffsets[index], window->y + 4);
        text->setString(text, FILE_CACHE.load(file), window->value);
    } else {
        text->setVisible(text, 0);
    }
}

/* Puts a window's string value of file on text, centred on the screen (string
   0x1F in palette 3), or hides text */
void CARDGAME_drawCenteredText(CardScreen *screen, CardScreenItems *items, CardWindow *window, TextWindow *text, s32 file) {
    TextTools tools;
    s32 width;

    if (window->value != 0) {
        text->setString(text, FILE_CACHE.load(file), window->value);
        initTextTools(&tools);
        width = tools.measure(text->text, text->style, text->spacingX);
        if (window->value == 0x1F) {
            text->setPalette(text, PALETTE_YELLOW);
        } else {
            text->setPalette(text, PALETTE_WHITE);
        }
        text->setVisible(text, 1);
        text->setPos(text, 0xA0 - width / 2, window->y + 4);
    } else {
        text->setVisible(text, 0);
    }
}

/* Opens (state 1), shows (2) or closes (3) window index, each its own way */
void CARDGAME_updateWindow(CardScreen *screen, CardScreenItems *items, CardWindow *window, s32 index) {
    if (window->state == 0) {
        return;
    }
    switch (window->state) {
    case 1:
    default:
        window->from = 0x1000 - (window->time << 12) / window->duration;
        window->time -= GFX.funcs.getFrameTime();
        if (window->time <= 0) {
            window->state = 2;
            switch (index) {
            case 0:
                CARDGAME_drawWindowText(screen, window, items->moreTexts[2], TEXT_FILE(TEXT_CARD_GAME), 1);
                break;
            case 2:
                CARDGAME_drawWindowText(screen, window, items->moreTexts[4], TEXT_FILE(TEXT_CARD_NAMES), 0);
                break;
            case 3:
                CARDGAME_drawWindowText(screen, window, items->moreTexts[3], TEXT_FILE(TEXT_CARD_GAME), 0);
                break;
            case 5:
                if (window->layout == 5) {
                    CARDGAME_drawCenteredText(screen, items, window, items->moreTexts[0], TEXT_FILE(TEXT_CARD_GAME));
                } else {
                    CARDGAME_drawWindowText(screen, window, items->moreTexts[0], TEXT_FILE(TEXT_CARD_GAME), window->value != 0x24);
                }
                break;
            case 1:
            case 4:
                break;
            }
        }
        break;
    case 2:
        window->from = 0x1000;
        switch (index) {
        case 2:
            CARDGAME_drawWindowText(screen, window, items->moreTexts[4], TEXT_FILE(TEXT_CARD_NAMES), 0);
            break;
        case 3:
            CARDGAME_drawWindowText(screen, window, items->moreTexts[3], TEXT_FILE(TEXT_CARD_GAME), 0);
            break;
        case 4:
            if (window->value == 0x1F4) {
                if (window->numbers[2] == 0) {
                    window->value = 0x2A;
                    CARDGAME_drawWindowText(screen, window, items->moreTexts[5], TEXT_FILE(TEXT_CARD_GAME), 1);
                } else {
                    window->value = 0x40;
                    CARDGAME_drawWindowNumbers(screen, items, window);
                    CARDGAME_drawWindowText(screen, window, items->moreTexts[5], TEXT_FILE(TEXT_CARD_GAME), 2);
                }
            } else {
                items->texts[0]->setVisible(items->texts[0], 0);
                items->texts[1]->setVisible(items->texts[1], 0);
                CARDGAME_drawWindowText(screen, window, items->moreTexts[5], TEXT_FILE(TEXT_CARD_EFFECTS), 0);
            }
            break;
        }
        break;
    case 3:
        switch (index) {
        case 0:
            items->moreTexts[2]->setVisible(items->moreTexts[2], 0);
            break;
        case 2:
            items->moreTexts[4]->setVisible(items->moreTexts[4], 0);
            break;
        case 3:
            items->moreTexts[3]->setVisible(items->moreTexts[3], 0);
            break;
        case 4:
            items->texts[0]->setVisible(items->texts[0], 0);
            items->texts[1]->setVisible(items->texts[1], 0);
            items->moreTexts[5]->setVisible(items->moreTexts[5], 0);
            break;
        case 5:
            items->moreTexts[0]->setVisible(items->moreTexts[0], 0);
            break;
        case 1:
            break;
        }
        window->time -= GFX.funcs.getFrameTime();
        if (window->time <= 0) {
            window->state = 0;
        }
        window->from = (window->time << 12) / window->duration;
        break;
    }
    CARDGAME_drawWindowFrame(screen, items, window);
}

/* Runs and draws the six windows */
void CARDGAME_updateWindows(CardScreen *screen, CardScreenItems *items) {
    s32 i;

    for (i = 0; i < 6; i++) {
        CARDGAME_updateWindow(screen, items, &screen->windows[i], i);
    }
}
