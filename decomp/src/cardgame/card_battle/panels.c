/* CARDGAME's panels, one a side, with their lights, bars, numbers and
   icons, and the blinkers. */

#include "cardgame/card_battle.h"

/* Draws a side's panel image of its wins, its parts blinking by its flags,
   and its frame */
static inline void drawPanelParts(CardPanel *panel, s32 side, s32 y, s32 imageY) {
    SpriteDrawer drawer;
    s32 t;
    s32 frame;
    s32 boxFrame;
    s32 iconT;
    s32 iconFrame;
    s32 i;

    initSpriteDrawer(&drawer);
    drawer.setLayerId(0x100, 1);
    drawer.setTexture(0x280, 0);
    drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 2), CARDGAME_panelLayouts[side].winsSprite + panel->wins, panel->winsX, imageY);
    t = panel->blinkTime % 30;
    if (panel->flags[7] != 0) {
        frame = t / 6;
    } else {
        frame = 0;
    }
    drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 2), CARDGAME_flag7Frames[frame] + 9, panel->x + CARDGAME_panelLayouts[side].flag7Part.x, y + CARDGAME_panelLayouts[side].flag7Part.y);
    t = panel->blinkTime % 30;
    if (panel->flags[6] != 0) {
        frame = t / 6;
    } else {
        frame = 0;
    }
    drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 2), CARDGAME_flag6Frames[frame] + 6, panel->x + CARDGAME_panelLayouts[side].flag6Part.x, y + CARDGAME_panelLayouts[side].flag6Part.y);
    t = panel->blinkTime % 30;
    if (panel->flags[5] != 0) {
        boxFrame = t / 6;
    } else {
        boxFrame = 0;
    }
    drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 2), CARDGAME_boxFrames[boxFrame] + 0x31, panel->boxX, panel->boxY);
    iconT = panel->blinkTime % 30;
    for (i = 0; i < 5; i++) {
        if (panel->flags[i] != 0) {
            iconFrame = iconT / 6;
        } else {
            iconFrame = 0;
        }
        drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 2), CARDGAME_flagIconSprites[i] + CARDGAME_flagIconFrames[iconFrame], panel->x + CARDGAME_panelLayouts[side].flagIcons.x + i * 0x2A, y + CARDGAME_panelLayouts[side].flagIcons.y);
    }
    drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 2), CARDGAME_panelLayouts[side].boxLabelSprite, panel->boxX + CARDGAME_panelLayouts[side].boxLabel.x, panel->boxY + CARDGAME_panelLayouts[side].boxLabel.y);
    drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 2), CARDGAME_panelLayouts[side].frame, panel->x, y);
}

/* Draws a side's panel (CardScreen.panels): its lights, bars, numbers and
   parts (drawPanelParts), which blink by its flags */
void CARDGAME_drawPanel(CardPanel *panel, CardScreenItems *items, s32 side) {
    s32 y;
    s32 imageY;

    y = panel->y;
    imageY = panel->winsY;
#if PAL_VIDEO
    if (SHIFT_PAL_SCREEN) {
        if (side == 0) {
            y += 12;
            imageY += 12;
        } else {
            y -= 12;
            imageY -= 12;
        }
    }
#endif
    /* the lights */
    {
        SpriteDrawer drawer;
        s32 t;
        s32 i;

        t = panel->blinkTime % 36;
        initSpriteDrawer(&drawer);
        drawer.setLayerId(0x100, 1);
        drawer.setTexture(0x340, 0);
        drawer.setClutRow(CARDGAME_panelLightCluts[t / 6]);
        for (i = 0; i < 5; i++) {
            if (panel->flags[i] != 0) {
                drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), 0xD, panel->x + CARDGAME_panelLayouts[side].flagLights.x + i * 0x2A, y + CARDGAME_panelLayouts[side].flagLights.y);
            }
        }
        if (panel->flags[8] != 0) {
            drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), 0xC, panel->x + CARDGAME_panelLayouts[side].apLight.x, y + CARDGAME_panelLayouts[side].apLight.y);
        }
        if (panel->flags[9] != 0) {
            drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), 0xC, panel->x + CARDGAME_panelLayouts[side].hpLight.x, y + CARDGAME_panelLayouts[side].hpLight.y);
        }
        if (panel->flags[6] != 0) {
            drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), 0xE, panel->x + CARDGAME_panelLayouts[side].flag6Light.x, y + CARDGAME_panelLayouts[side].flag6Light.y);
        }
        if (panel->flags[7] != 0) {
            drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), 0xE, panel->x + CARDGAME_panelLayouts[side].flag7Light.x, y + CARDGAME_panelLayouts[side].flag7Light.y);
        }
        if (panel->flags[5] != 0) {
            drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), 0xF, panel->boxX + CARDGAME_panelLayouts[side].boxLight.x, panel->boxY + CARDGAME_panelLayouts[side].boxLight.y);
        }
    }
    /* the bars and the numbers */
    {
        SpriteDrawer drawer;
        CardNumber number;
        s32 t;
        s32 i;

        t = panel->blinkTime % 24;
        initSpriteDrawer(&drawer);
        drawer.setLayerId(0x100, 1);
        drawer.setTexture(0x340, 0);
        if (panel->flags[8] != 0) {
            drawer.setClutRow(t / 6 + 1);
        } else {
            drawer.setClutRow(0);
        }
#if VERSION_US
        drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), CARDGAME_panelBarSprites[1][0], panel->x + CARDGAME_panelLayouts[side].apBar.x, y + CARDGAME_panelLayouts[side].apBar.y);
#elif VERSION_EU || VERSION_JP
        drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), CARDGAME_panelBarSprites[LANGUAGE][0], panel->x + CARDGAME_panelLayouts[side].apBar.x, y + CARDGAME_panelLayouts[side].apBar.y);
#endif
        if (panel->flags[9] != 0) {
            drawer.setClutRow(t / 6 + 1);
        } else {
            drawer.setClutRow(0);
        }
#if VERSION_US
        drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), CARDGAME_panelBarSprites[1][1], panel->x + CARDGAME_panelLayouts[side].hpBar.x, y + CARDGAME_panelLayouts[side].hpBar.y);
#elif VERSION_EU || VERSION_JP
        drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), CARDGAME_panelBarSprites[LANGUAGE][1], panel->x + CARDGAME_panelLayouts[side].hpBar.x, y + CARDGAME_panelLayouts[side].hpBar.y);
#endif

        number.depth = 1;
        number.x = panel->x + CARDGAME_panelLayouts[side].deckCount.x;
        number.y = CARDGAME_panelLayouts[side].deckCount.y + y;
        number.digits = 2;
        number.leadingZeros = 1;
        number.value = panel->deckCount;
        CARDGAME_drawNumber(&number, 0);
        number.x = panel->x + CARDGAME_panelLayouts[side].handCount.x;
        number.y = CARDGAME_panelLayouts[side].handCount.y + y;
        number.digits = 2;
        number.leadingZeros = 1;
        number.value = panel->handCount;
        CARDGAME_drawNumber(&number, 0);
        number.x = panel->x + CARDGAME_panelLayouts[side].apTotal.x;
        number.y = CARDGAME_panelLayouts[side].apTotal.y + y;
        number.digits = 3;
        number.leadingZeros = 0;
        number.value = panel->apTotal;
        CARDGAME_drawNumber(&number, 0);
        number.x = panel->x + CARDGAME_panelLayouts[side].hpTotal.x;
        number.y = CARDGAME_panelLayouts[side].hpTotal.y + y;
        number.digits = 3;
        number.leadingZeros = 0;
        number.value = panel->hpTotal;
        CARDGAME_drawNumber(&number, 0);
        for (i = 0; i < 5; i++) {
            number.x = panel->x + CARDGAME_panelLayouts[side].flagNumbers.x + i * 0x2A;
            number.y = CARDGAME_panelLayouts[side].flagNumbers.y + y;
            number.digits = 2;
            number.leadingZeros = 1;
            number.value = panel->points[i];
            CARDGAME_drawNumber(&number, 0);
        }
        number.x = panel->boxX + CARDGAME_panelLayouts[side].discardCount.x;
        number.y = panel->boxY + CARDGAME_panelLayouts[side].discardCount.y;
        number.digits = 2;
        number.leadingZeros = 1;
        number.value = panel->discardCount;
        CARDGAME_drawNumber(&number, 0);
    }
    drawPanelParts(panel, side, y, imageY);
    panel->blinkTime = (panel->blinkTime + GFX.funcs.getFrameTime()) & 0xFFFF;
}

/* Slides a side's whole panel in from its hidden places to its shown ones,
   where it stays (state 2) */
static inline void slidePanelIn(CardPanel *panel, s32 shownX, s32 shownY, s32 hiddenX, s32 hiddenY,
                                 s32 shownBoxX, s32 hiddenBoxX, s32 shownBoxY, s32 hiddenBoxY,
                                 s32 shownWinsX, s32 hiddenWinsX, s32 shownWinsY, s32 hiddenWinsY) {
    if ((panel->time -= GFX.funcs.getFrameTime()) > 0) {
        panel->x = shownX - (shownX - hiddenX) * panel->time / panel->duration;
        panel->y = shownY - (shownY - hiddenY) * panel->time / panel->duration;
        panel->boxX = shownBoxX - (shownBoxX - hiddenBoxX) * panel->time / panel->duration;
        panel->boxY = shownBoxY - (shownBoxY - hiddenBoxY) * panel->time / panel->duration;
        panel->winsX = shownWinsX - (shownWinsX - hiddenWinsX) * panel->time / panel->duration;
        panel->winsY = shownWinsY - (shownWinsY - hiddenWinsY) * panel->time / panel->duration;
    } else {
        panel->state = 2;
        panel->time = 0;
        panel->duration = 0;
        panel->x = shownX;
        panel->y = shownY;
        panel->boxX = shownBoxX;
        panel->boxY = shownBoxY;
        panel->winsX = shownWinsX;
        panel->winsY = shownWinsY;
    }
}

/* Slides a side's whole panel out from its shown places to its hidden ones,
   where it's gone (state 0) */
static inline void slidePanelOut(CardPanel *panel, s32 shownX, s32 shownY, s32 hiddenX, s32 hiddenY,
                                  s32 shownBoxX, s32 hiddenBoxX, s32 shownBoxY, s32 hiddenBoxY,
                                  s32 shownWinsX, s32 hiddenWinsX, s32 shownWinsY, s32 hiddenWinsY) {
    if ((panel->time -= GFX.funcs.getFrameTime()) > 0) {
        panel->x = hiddenX - (hiddenX - shownX) * panel->time / panel->duration;
        panel->y = hiddenY - (hiddenY - shownY) * panel->time / panel->duration;
        panel->boxX = hiddenBoxX - (hiddenBoxX - shownBoxX) * panel->time / panel->duration;
        panel->boxY = hiddenBoxY - (hiddenBoxY - shownBoxY) * panel->time / panel->duration;
        panel->winsX = hiddenWinsX - (hiddenWinsX - shownWinsX) * panel->time / panel->duration;
        panel->winsY = hiddenWinsY - (hiddenWinsY - shownWinsY) * panel->time / panel->duration;
    } else {
        panel->state = 0;
        panel->event = 1;
        panel->time = 0;
        panel->duration = 0;
        panel->x = hiddenX;
        panel->y = hiddenY;
        panel->boxX = hiddenBoxX;
        panel->boxY = hiddenBoxY;
        panel->winsX = hiddenWinsX;
        panel->winsY = hiddenWinsX; /* not hiddenWinsY */
    }
}

/* Moves a side's panel between its shown and hidden places: in (state 1,
   slidePanelIn), out (3, slidePanelOut), or only x/y in or out (4, 5) */
void CARDGAME_slidePanel(CardPanel *panel, CardScreenItems *items, s32 side) {
    s32 shownX;
    s32 shownY;
    s32 hiddenX;
    s32 hiddenY;
    s32 shownBoxX;
    s32 hiddenBoxX;
    s32 shownBoxY;
    s32 hiddenBoxY;
    s32 shownWinsX;
    s32 hiddenWinsX;
    s32 shownWinsY;
    s32 hiddenWinsY;

    if (side == 0) {
        shownX = 0;
        shownY = 0x8D;
        hiddenX = 0;
        hiddenY = 0xF1;
        shownBoxX = 0x120;
        shownBoxY = 0x8F;
        hiddenBoxX = 0x147;
        hiddenBoxY = 0x8F;
        shownWinsX = 0x113;
        shownWinsY = 0xBD;
        hiddenWinsY = 0xBD;
        /* the match depends on setting it in both branches, not once after them */
        hiddenWinsX = 0x13A;
    } else {
        shownX = 0;
        shownY = 0;
        hiddenX = 0;
        hiddenY = -100;
        shownBoxX = 0x120;
        shownBoxY = 0x50;
        hiddenBoxX = 0x147;
        hiddenBoxY = 0x50;
        shownWinsX = 0x113;
        shownWinsY = 0x26;
        hiddenWinsY = 0x26;
        hiddenWinsX = 0x13A;
    }
    /* the match depends on the empty case 0 */
    switch (panel->event) {
    case 1:
        panel->event = 0;
        break;
    case 2:
        panel->event = 0;
        break;
    case 0:
        break;
    }
    switch (panel->state) {
    case 1:
        slidePanelIn(panel, shownX, shownY, hiddenX, hiddenY, shownBoxX, hiddenBoxX, shownBoxY, hiddenBoxY,
                     shownWinsX, hiddenWinsX, shownWinsY, hiddenWinsY);
        break;
    case 3:
        slidePanelOut(panel, shownX, shownY, hiddenX, hiddenY, shownBoxX, hiddenBoxX, shownBoxY, hiddenBoxY,
                      shownWinsX, hiddenWinsX, shownWinsY, hiddenWinsY);
        break;
    case 4:
        panel->boxX = hiddenBoxX;
        panel->boxY = hiddenBoxY;
        panel->winsX = hiddenWinsX;
        panel->winsY = hiddenWinsX;
        if ((panel->time -= GFX.funcs.getFrameTime()) > 0) {
            panel->x = shownX - (shownX - hiddenX) * panel->time / panel->duration;
            panel->y = shownY - (shownY - hiddenY) * panel->time / panel->duration;
        } else {
            panel->state = 2;
            panel->time = 0;
            panel->duration = 0;
            panel->x = shownX;
            panel->y = shownY;
        }
        break;
    case 5:
        panel->boxX = hiddenBoxX;
        panel->boxY = hiddenBoxY;
        panel->winsX = hiddenWinsX;
        panel->winsY = hiddenWinsX;
        if ((panel->time -= GFX.funcs.getFrameTime()) > 0) {
            panel->x = hiddenX - (hiddenX - shownX) * panel->time / panel->duration;
            panel->y = hiddenY - (hiddenY - shownY) * panel->time / panel->duration;
        } else {
            panel->state = 0;
            panel->event = 1;
            panel->time = 0;
            panel->duration = 0;
            panel->x = hiddenX;
            panel->y = hiddenY;
        }
        break;
    case 0:
    case 2:
        break;
    }
}

/* Draws a side's panel icon at the panel's open or close scale, and once the
   panel is open (state 2) its label; the European version takes both
   positions from CARDGAME_panelIconPositions, by SHIFT_PAL_SCREEN */
void CARDGAME_drawPanelIcon(CardScreen *screen, CardScreenItems *items, s32 side, CardPanelScale *scale) {
    SpriteDrawer drawer;
    s32 iconX;
    s32 iconY;
    s32 textX;
    s32 textY;

#if PAL_VIDEO
    if (side == 0) {
        iconX = CARDGAME_panelIconPositions[SHIFT_PAL_SCREEN][0][0].x;
        iconY = CARDGAME_panelIconPositions[SHIFT_PAL_SCREEN][0][0].y;
        textX = CARDGAME_panelIconPositions[SHIFT_PAL_SCREEN][1][0].x;
        textY = CARDGAME_panelIconPositions[SHIFT_PAL_SCREEN][1][0].y;
    } else {
        iconX = CARDGAME_panelIconPositions[SHIFT_PAL_SCREEN][0][1].x;
        iconY = CARDGAME_panelIconPositions[SHIFT_PAL_SCREEN][0][1].y;
        textX = CARDGAME_panelIconPositions[SHIFT_PAL_SCREEN][1][1].x;
        textY = CARDGAME_panelIconPositions[SHIFT_PAL_SCREEN][1][1].y;
    }
#else
    if (side == 0) {
        iconX = 0x7C;
        iconY = 0x33;
        textX = 0x8A;
        textY = 0x34;
    } else {
        iconX = 0x7C;
        iconY = 0x23;
        textX = 0x8A;
        textY = 0x24;
    }
#endif
    if (scale->state != 0) {
        initSpriteDrawer(&drawer);
        drawer.setLayerId(0x100, 1);
        drawer.setTexture(0x280, 0);
        drawer.setPivot(screen->panels[side].x + iconX, screen->panels[side].y + iconY);
        drawer.setScale(scale->value, 0x1000, 0x1000);
        drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 2), side == 0 ? 0x44 : 0x43,
                    screen->panels[side].x + iconX, screen->panels[side].y + iconY);
    }
    if (scale->state == 2) {
        items->panelTexts[side]->setPos(items->panelTexts[side], screen->panels[side].x + textX,
                                        screen->panels[side].y + textY);
        items->panelTexts[side]->setString(items->panelTexts[side], FILE_CACHE.load(TEXT_FILE(TEXT_CARD_GAME)), 0x3F);
    } else {
        items->panelTexts[side]->setVisible(items->panelTexts[side], 0);
    }
}

/* Grows (state 1) or shrinks (3) side's panel icon, and draws it */
void CARDGAME_updatePanelIcon(CardScreen *screen, CardScreenItems *items, s32 side, CardPanelScale *scale) {
    switch (scale->state) {
    case 1:
        scale->value = 0x1000 - (scale->time << 12) / scale->duration;
        scale->time -= GFX.funcs.getFrameTime();
        if (scale->time <= 0) {
            scale->state = 2;
        }
        break;
    case 3:
        scale->value = (scale->time << 12) / scale->duration;
        scale->time -= GFX.funcs.getFrameTime();
        if (scale->time <= 0) {
            scale->state = 0;
        }
        break;
    case 0:
        break;
    case 2:
        break;
    }
    CARDGAME_drawPanelIcon(screen, items, side, scale);
}

/* Moves a blinker towards its target; it stops there (state 1) */
void CARDGAME_moveBlinker(CardBlinker *blinker) {
    blinker->time -= GFX.funcs.getFrameTime();
    if (blinker->time != 0) {
        blinker->x = blinker->targetX - (blinker->targetX - blinker->startX) * blinker->time / blinker->duration;
        blinker->y = blinker->targetY - (blinker->targetY - blinker->startY) * blinker->time / blinker->duration;
    } else {
        blinker->state = 1;
        blinker->duration = 0;
        blinker->time = 0;
        blinker->x = blinker->targetX;
        blinker->y = blinker->targetY;
    }
}

/* Draws a blinker, its CLUT row changing every four frames */
void CARDGAME_drawBlinker(CardBlinker *blinker) {
    SpriteDrawer drawer;
    s32 row;

    initSpriteDrawer(&drawer);
    drawer.setLayerId(0x100, 1);
    drawer.setTexture(0x340, 0);
    switch (++blinker->blinkTime >> 2) {
    default:
        blinker->blinkTime = 0;
        /* fallthrough */
    case 0:
        row = 0;
        break;
    case 1:
        row = 3;
        break;
    case 2:
        row = 1;
        break;
    }
    drawer.setClutRow(row);
    drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), 8, blinker->x, blinker->y);
}

/* Moves and draws the twelve blinkers */
void CARDGAME_updateBlinkers(CardScreen *screen, CardScreenItems *items) {
    u32 i;
    s32 two;

    i = 0;
    two = 2;
    for (; i < 12; i++) {
        if (screen->blinkers[i].state != 0) {
            if (screen->blinkers[i].state == two) {
                CARDGAME_moveBlinker(&screen->blinkers[i]);
            }
            CARDGAME_drawBlinker(&screen->blinkers[i]);
        }
    }
}

/* Runs and draws both panels and their icons */
void CARDGAME_updatePanels(CardScreen *screen, CardScreenItems *items) {
    CARDGAME_updatePanelIcon(screen, items, 0, &screen->panels[0].scale);
    CARDGAME_updatePanelIcon(screen, items, 1, &screen->panels[1].scale);
    CARDGAME_slidePanel(&screen->panels[0], items, 0);
    CARDGAME_slidePanel(&screen->panels[1], items, 1);
    CARDGAME_drawPanel(&screen->panels[0], items, 0);
    CARDGAME_drawPanel(&screen->panels[1], items, 1);
}
