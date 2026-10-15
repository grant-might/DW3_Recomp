/* CARDGAME's card screen (CARDGAME_createScreen): its task and the
   functions the battle calls on it. */

#include "cardgame/card_battle.h"

/* The card screen's task: makes its cursor and text windows, then each frame
   updates its blinkers, windows, sprites, gauges, background and panels */
void CARDGAME_updateScreen(CardScreen *screen, CardScreenItems *items) {
    switch (screen->state) {
    case 0:
    default:
        screen->nextState(screen);
        items->cursor = createCursor(0x100, 0, 0, 0);
        items->cursor->setVisible(items->cursor, 0);
        items->texts[0] = createTextWindow(0x100, 1, 0, 0);
        items->texts[1] = createTextWindow(0x100, 1, 0, 0);
        items->texts[2] = createTextWindow(0x100, 1, 0, 0);
        items->panelTexts[0] = createTextWindow(0x100, 1, 0, 0);
        items->panelTexts[1] = createTextWindow(0x100, 1, 0, 0);
        items->moreTexts[0] = createTextWindow(0x100, 1, 0, 0);
        items->moreTexts[1] = createTextWindow(0x100, 1, 0, 0);
        items->moreTexts[1]->setLines(items->moreTexts[1], 3);
        items->moreTexts[2] = createTextWindow(0x100, 1, 0, 0);
        items->moreTexts[3] = createTextWindow(0x100, 1, 0, 0);
        items->moreTexts[4] = createTextWindow(0x100, 1, 0, 0);
        items->moreTexts[5] = createTextWindow(0x100, 1, 0, 0);
        items->moreTexts[5]->setLines(items->moreTexts[5], 3);
        items->moreTexts[6] = createTextWindow(0x100, 1, 0, 0);
        items->moreTexts[6]->setLines(items->moreTexts[6], 5);
        items->moreTexts[7] = createTextWindow(0x100, 1, 0, 0);
        items->moreTexts[7]->setLines(items->moreTexts[7], 2);
        break;
    case 1:
        screen->time += GFX.funcs.getFrameTime();
        screen->spriteFlags = 0;
        CARDGAME_updateBlinkers(screen, items);
        CARDGAME_updateMenuWindow(screen, items);
        CARDGAME_updateMessageWindow(screen, items);
        CARDGAME_updateWindows(screen, items);
        CARDGAME_updateSprites(screen, items);
        CARDGAME_updateGauges(screen, items);
        CARDGAME_drawBackground(screen);
        CARDGAME_updatePanels(screen, items);
        break;
    case 3:
        break;
    }
}

/* Opens gauge index showing value, over 10 frames */
void CARDGAME_openGauge(CardScreen *screen, s32 index, s16 value) {
    screen->gauges[index].from = 0x1000;
    screen->gauges[index].state = 1;
    screen->gauges[index].to = 0;
    screen->gauges[index].value = value;
    screen->gauges[index].duration = 10;
    screen->gauges[index].time = 10;
}

/* Closes gauge index, over 5 frames */
void CARDGAME_closeGauge(CardScreen *screen, s32 index) {
    screen->gauges[index].from = 0x1000;
    screen->gauges[index].to = 0x1000;
    screen->gauges[index].state = 3;
    screen->gauges[index].duration = 5;
    screen->gauges[index].time = 5;
}

/* Opens window index (CardScreen.windows) at x, y, with its layout and value */
void CARDGAME_openWindow(CardScreen *screen, s32 index, s16 layout, s32 value, s32 x, s32 y) {
    SOUND.playSound(SOUND_MENU_OPEN);
    screen->windows[index].x = x;
    screen->windows[index].y = y;
    screen->windows[index].from = 0;
    screen->windows[index].to = 0x1000;
    screen->windows[index].state = 1;
    screen->windows[index].layout = layout;
    screen->windows[index].value = value;
    screen->windows[index].duration = 12;
    screen->windows[index].time = 12;
}

/* Closes window index */
void CARDGAME_closeWindow(CardScreen *screen, s32 index) {
    SOUND.playSound(SOUND_MENU_CLOSE);
    screen->windows[index].from = 0x1000;
    screen->windows[index].to = 0x1000;
    screen->windows[index].state = 3;
    screen->windows[index].duration = 6;
    screen->windows[index].time = 6;
}

/* Shows blinker index, still, at x, y. Nothing calls this method or
   CARDGAME_startBlinkerMove. */
s32 CARDGAME_showBlinker(CardScreen *screen, s32 index, s16 x, s16 y) {
    screen->blinkers[index].state = 1;
    screen->blinkers[index].x = x;
    screen->blinkers[index].y = y;
    screen->blinkers[index].targetY = 0;
    screen->blinkers[index].targetX = 0;
    screen->blinkers[index].startY = 0;
    screen->blinkers[index].startX = 0;
    screen->blinkers[index].duration = 0;
    screen->blinkers[index].time = 0;
    screen->blinkers[index].blinkTime = 0;
    return 0;
}

/* Starts moving blinker index to x, y over duration frames */
s32 CARDGAME_startBlinkerMove(CardScreen *screen, s32 index, u8 duration, s16 x, s32 y) {
    screen->blinkers[index].state = 2;
    screen->blinkers[index].targetX = x;
    screen->blinkers[index].duration = duration;
    screen->blinkers[index].time = duration;
    screen->blinkers[index].targetY = y;
    screen->blinkers[index].startX = screen->blinkers[index].x;
    screen->blinkers[index].startY = screen->blinkers[index].y;
    return 0;
}

/* Opens the message window at place with message, asking prompt with the
   cursor on choice */
void CARDGAME_openMessage(CardScreen *screen, s32 message, s32 prompt, s16 choice, s32 place) {
    SOUND.playSound(SOUND_MENU_OPEN);
    screen->message.duration = 12;
    screen->message.time = 12;
    screen->message.choice = choice;
    screen->message.message = message;
    screen->message.prompt = prompt;
    screen->message.place = place;
    screen->message.state = 1;
}

/* Closes the message window */
void CARDGAME_closeMessage(CardScreen *screen) {
    SOUND.playSound(SOUND_MENU_CLOSE);
    screen->message.duration = 6;
    screen->message.time = 6;
    screen->message.state = 5;
}

/* Confirms the message window's choice: the cursor nudges, then it closes */
void CARDGAME_confirmMessage(CardScreen *screen) {
    SOUND.playSound(SOUND_SELECT);
    screen->message.duration = 10;
    screen->message.time = 10;
    screen->message.state = 4;
}

/* Puts the message window's cursor on choice */
void CARDGAME_setMessageChoice(CardScreen *screen, s32 choice) {
    screen->message.choice = choice;
}

/* Opens the menu window with the cursor on row */
void CARDGAME_openMenuWindow(CardScreen *screen, s16 row) {
    SOUND.playSound(SOUND_MENU_OPEN);
    screen->menuDuration = 12;
    screen->menuTime = 12;
    screen->menuRow = row;
    screen->menuState = 1;
}

/* Closes the menu window */
void CARDGAME_closeMenuWindow(CardScreen *screen) {
    SOUND.playSound(SOUND_MENU_CLOSE);
    screen->menuDuration = 6;
    screen->menuTime = 6;
    screen->menuState = 5;
}

/* Confirms the menu window's row: the cursor nudges, then it closes */
void CARDGAME_confirmMenuWindow(CardScreen *screen) {
    SOUND.playSound(SOUND_SELECT);
    screen->menuDuration = 10;
    screen->menuTime = 10;
    screen->menuState = 4;
}

/* Puts the menu window's cursor on row */
void CARDGAME_setMenuWindowRow(CardScreen *screen, s16 row) {
    screen->menuRow = row;
}

/* Puts both panels away at once */
void CARDGAME_resetPanels(CardScreen *screen) {
    screen->panels[0].state = 0;
    screen->panels[0].duration = 0;
    screen->panels[0].time = 0;
    screen->panels[0].x = 0;
    screen->panels[0].y = 0xF1;
    screen->panels[0].boxX = 0x147;
    screen->panels[0].boxY = 0x8F;
    screen->panels[0].winsX = 0x140;
    screen->panels[0].winsY = 0xBD;
    screen->panels[1].state = 0;
    screen->panels[1].duration = 0;
    screen->panels[1].time = 0;
    screen->panels[1].x = 0;
    screen->panels[1].y = -100;
    screen->panels[1].boxX = 0x147;
    screen->panels[1].boxY = 0x50;
    screen->panels[1].winsX = 0x140;
    screen->panels[1].winsY = 0x26;
}

/* Slides both panels in, over 20 frames */
void CARDGAME_openPanels(CardScreen *screen) {
    screen->panels[0].state = 1;
    screen->panels[0].event = 2;
    screen->panels[0].duration = 20;
    screen->panels[0].time = 20;
    screen->panels[0].x = 0;
    screen->panels[0].y = 0xF1;
    screen->panels[0].boxX = 0x147;
    screen->panels[0].boxY = 0x8F;
    screen->panels[0].winsX = 0x140;
    screen->panels[0].winsY = 0xBD;
    screen->panels[1].state = 1;
    screen->panels[1].duration = 20;
    screen->panels[1].time = 20;
    screen->panels[1].x = 0;
    screen->panels[1].y = -100;
    screen->panels[1].boxX = 0xF9;
    screen->panels[1].boxY = 0x50;
    screen->panels[1].winsX = 0x140;
    screen->panels[1].winsY = 0x26;
}

/* Slides both panels out, over 10 frames */
void CARDGAME_closePanels(CardScreen *screen) {
    screen->panels[0].state = 3;
    screen->panels[0].duration = 10;
    screen->panels[0].time = 10;
    screen->panels[0].x = 0;
    screen->panels[0].y = 0x8D;
    screen->panels[0].boxX = 0x120;
    screen->panels[0].boxY = 0x8F;
    screen->panels[0].winsX = 0x113;
    screen->panels[0].winsY = 0xBD;
    screen->panels[1].state = 3;
    screen->panels[1].duration = 10;
    screen->panels[1].time = 10;
    screen->panels[1].x = 0;
    screen->panels[1].y = 0;
    screen->panels[1].boxX = 0x120;
    screen->panels[1].boxY = 0x50;
    screen->panels[1].winsX = 0x113;
    screen->panels[1].winsY = 0x26;
}

/* Slides side's panel in over 10 frames, stopping the other's */
void CARDGAME_openPanel(CardScreen *screen, s32 side) {
    screen->panels[side].state = 4;
    screen->panels[side ^ 1].state = 0;
    screen->panels[side].event = 2;
    screen->panels[side].duration = 10;
    screen->panels[side].time = 10;
    if (side == 0) {
        screen->panels[0].x = 0;
        screen->panels[0].y = 0xF1;
    } else {
        screen->panels[side].x = 0;
        screen->panels[side].y = -100;
    }
}

/* Slides side's panel out over 5 frames, stopping the other's */
void CARDGAME_closePanel(CardScreen *screen, s32 side) {
    screen->panels[side].state = 5;
    screen->panels[side ^ 1].state = 0;
    screen->panels[side].duration = 5;
    screen->panels[side].time = 5;
    if (side == 0) {
        screen->panels[0].x = 0;
        screen->panels[0].y = 0x8D;
    } else {
        screen->panels[side].x = 0;
        screen->panels[side].y = 0;
    }
}

/* Grows side's panel icon over 12 frames */
void CARDGAME_openPanelIcon(CardScreen *screen, s32 side) {
    screen->panels[side].scale.state = 1;
    screen->panels[side].scale.duration = 12;
    screen->panels[side].scale.time = 12;
    screen->panels[side].scale.value = 0;
}

/* Shrinks side's panel icon over 6 frames */
void CARDGAME_closePanelIcon(CardScreen *screen, s32 side) {
    screen->panels[side].scale.state = 3;
    screen->panels[side].scale.duration = 6;
    screen->panels[side].scale.time = 6;
    screen->panels[side].scale.value = 0x1000;
}

/* CardScreen.addSprite: puts a card sprite, still and at full size, at x, y */
s32 CARDGAME_addSprite(CardScreen *screen, s32 index, s32 x, s32 y) {
    HEAP.zero(&screen->sprites[index], sizeof(CardSprite));
    screen->sprites[index].visible = 1;
    screen->sprites[index].state = 1;
    screen->sprites[index].x = x;
    screen->sprites[index].y = y;
    screen->sprites[index].targetScaleX = 0x1000;
    screen->sprites[index].scaleX = 0x1000;
    screen->sprites[index].targetScaleY = 0x1000;
    screen->sprites[index].scaleY = 0x1000;
    screen->sprites[index].index = 0;
    screen->sprites[index].color = 0;
    screen->sprites[index].slot = index;
    screen->sprites[index].ap = 0;
    screen->sprites[index].hp = 0;
    screen->sprites[index].effect = 0;
    screen->sprites[index].dimmed = 0;
    screen->sprites[index].moving = 0;
    return 0;
}

/* Starts turning a sprite over (state 4, CARDGAME_flipSprite) */
s32 CARDGAME_startSpriteFlip(CardScreen *screen, s32 index) {
    SOUND.playSound(0x8004613E);
    screen->sprites[index].state = 4;
    screen->sprites[index].time = 0;
    screen->sprites[index].duration = 0;
    screen->sprites[index].scaleX = 0x1000;
    screen->sprites[index].scaleY = 0x1000;
    screen->sprites[index].moving = 0;
    return 0;
}

/* Starts a sprite's jitter (state 6, CARDGAME_jitterSprite), with effect 1 */
s32 CARDGAME_startSpriteJitter(CardScreen *screen, s32 index) {
    SOUND.playSound(0x9C0003);
    screen->sprites[index].state = 6;
    screen->sprites[index].scaleX = 0x1000;
    screen->sprites[index].scaleY = 0x1000;
    screen->sprites[index].time = 0;
    screen->sprites[index].duration = 0;
    screen->sprites[index].effect = 1;
    screen->sprites[index].moving = 0;
    screen->sprites[index].startX = screen->sprites[index].x;
    screen->sprites[index].startY = screen->sprites[index].y;
    return 0;
}

/* Starts a sprite's shake (state 7, CARDGAME_shakeSprite), with effect 1 */
s32 CARDGAME_startSpriteShake(CardScreen *screen, s32 index) {
    SOUND.playSound(0x9C0003);
    screen->sprites[index].state = 7;
    screen->sprites[index].time = 0;
    screen->sprites[index].duration = 0;
    screen->sprites[index].effect = 1;
    screen->sprites[index].moving = 0;
    screen->sprites[index].startX = screen->sprites[index].x;
    screen->sprites[index].startY = screen->sprites[index].y;
    return 0;
}

/* Starts a sprite's effect 2 with the recovery sound (state 8), and then its
   blink */
s32 CARDGAME_startSpriteRecovery(CardScreen *screen, s32 index) {
    SOUND.playSound(SOUND_RECOVERY);
    screen->sprites[index].state = 8;
    screen->sprites[index].time = 0;
    screen->sprites[index].duration = 0;
    screen->sprites[index].effect = 2;
    screen->sprites[index].moving = 0;
    screen->sprites[index].startX = screen->sprites[index].x;
    screen->sprites[index].startY = screen->sprites[index].y;
    return 0;
}

/* Starts a sprite's effect 3 (which 0) for 36 frames, or 4 for 28 */
s32 CARDGAME_startSpriteEffect(CardScreen *screen, s32 index, s32 which) {
    switch (which) {
    case 0:
    default:
        screen->sprites[index].state = 9;
        screen->sprites[index].effect = 3;
        break;
    case 1:
        screen->sprites[index].effect = 4;
        screen->sprites[index].state = 10;
        break;
    }
    screen->sprites[index].time = 0;
    screen->sprites[index].duration = 0;
    screen->sprites[index].moving = 0;
    screen->sprites[index].startX = screen->sprites[index].x;
    screen->sprites[index].startY = screen->sprites[index].y;
    return 0;
}

/* Scales a sprite to scaleX, scaleY: at once when instant is 1, else over
   duration (state 2) */
s32 CARDGAME_setSpriteScaleTarget(CardScreen *screen, s32 index, s32 duration, s32 scaleX, s32 scaleY, s32 instant) {
    screen->sprites[index].targetScaleX = scaleX;
    screen->sprites[index].targetScaleY = scaleY;
    screen->sprites[index].startScaleX = screen->sprites[index].scaleX;
    screen->sprites[index].startScaleY = screen->sprites[index].scaleY;
    if (instant != 1) {
        screen->sprites[index].duration = duration;
        screen->sprites[index].time = duration;
        screen->sprites[index].state = 2;
        screen->sprites[index].moving = 1;
        screen->sprites[index].targetX = screen->sprites[index].x;
        screen->sprites[index].targetY = screen->sprites[index].y;
    }
    return 0;
}

/* CardScreen.setSpriteScale: scales a sprite at once */
void CARDGAME_setSpriteScale(CardScreen *screen, s32 index, s32 scaleX, s32 scaleY) {
    CARDGAME_setSpriteScaleTarget(screen, index, 0, scaleX, scaleY, 1);
}

/* CardScreen.scaleSprite: scales a sprite over duration */
void CARDGAME_scaleSprite(CardScreen *screen, s32 index, s32 duration, s32 scaleX, s32 scaleY) {
    CARDGAME_setSpriteScaleTarget(screen, index, duration, scaleX, scaleY, 0);
}

/* Sets a sprite's move to x, y over duration, from where it is */
void CARDGAME_setSpriteMove(CardScreen *screen, s32 index, s32 duration, s32 x, s32 y) {
    screen->sprites[index].targetX = x;
    screen->sprites[index].duration = duration;
    screen->sprites[index].time = duration;
    screen->sprites[index].targetY = y;
    screen->sprites[index].startX = screen->sprites[index].x;
    screen->sprites[index].startY = screen->sprites[index].y;
}

/* Starts moving a sprite to x, y over duration (state 2), with a sound at
   the start and the end */
void CARDGAME_startSpriteMove(CardScreen *screen, s32 index, s32 duration, s32 x, s32 y) {
    SOUND.playSound(0x8004603C);
    screen->sprites[index].state = 2;
    CARDGAME_setSpriteMove(screen, index, duration, x, y);
}

/* Starts moving a sprite to x, y over duration without sounds (state 3) */
void CARDGAME_startSpriteSlide(CardScreen *screen, s32 index, s32 duration, s32 x, s32 y) {
    screen->sprites[index].state = 3;
    CARDGAME_setSpriteMove(screen, index, duration, x, y);
}

/* Starts a sprite's flight to x, y and back (state 5, CARDGAME_flySprite),
   after it grows for 16 frames */
s32 CARDGAME_startSpriteFly(CardScreen *screen, s32 index, s32 duration, s32 x, s32 y) {
    screen->sprites[index].state = 5;
    screen->sprites[index].growTime = 16;
    screen->sprites[index].scaleX = 0x1000;
    screen->sprites[index].scaleY = 0x1000;
    screen->sprites[index].targetX = x;
    screen->sprites[index].duration = duration;
    screen->sprites[index].time = duration;
    screen->sprites[index].targetY = y;
    screen->sprites[index].startX = screen->sprites[index].x;
    screen->sprites[index].startY = screen->sprites[index].y;
    return 0;
}

/* CardScreen.setPanelValue: sets one of side's panel values: its points (0
   to 4), its deck, hand and discard counts, its AP or its HP (CARD_PANEL_*) */
void CARDGAME_setPanelValue(CardScreen *screen, s32 side, u32 which, s32 value) {
    switch (which) {
    case 0:
        screen->panels[side].points[0] = value;
        break;
    case 1:
        screen->panels[side].points[1] = value;
        break;
    case 2:
        screen->panels[side].points[2] = value;
        break;
    case 3:
        screen->panels[side].points[3] = value;
        break;
    case 4:
        screen->panels[side].points[4] = value;
        break;
    case CARD_PANEL_DECK:
        screen->panels[side].deckCount = value;
        break;
    case CARD_PANEL_HAND:
        screen->panels[side].handCount = value;
        break;
    case CARD_PANEL_DISCARDS:
        screen->panels[side].discardCount = value;
        break;
    case CARD_PANEL_AP:
        screen->panels[side].apTotal = value;
        break;
    case CARD_PANEL_HP:
        screen->panels[side].hpTotal = value;
        break;
    }
}

/* CardScreen.setPanelFlags: sets the panels' blink flags from bits, two to a
   flag: the even bits the player's, the odd ones the opponent's */
void CARDGAME_setPanelFlags(CardScreen *screen, s32 bits) {
    if (bits & 1) {
        screen->panels[0].flags[0] = 1;
    }
    if (bits & 4) {
        screen->panels[0].flags[1] = 1;
    }
    if (bits & 0x10) {
        screen->panels[0].flags[2] = 1;
    }
    if (bits & 0x40) {
        screen->panels[0].flags[3] = 1;
    }
    if (bits & 0x100) {
        screen->panels[0].flags[4] = 1;
    }
    if (bits & 0x400) {
        screen->panels[0].flags[5] = 1;
    }
    if (bits & 0x1000) {
        screen->panels[0].flags[6] = 1;
    }
    if (bits & 0x4000) {
        screen->panels[0].flags[7] = 1;
    }
    if (bits & 0x10000) {
        screen->panels[0].flags[8] = 1;
    }
    if (bits & 0x40000) {
        screen->panels[0].flags[9] = 1;
    }
    if (bits & 2) {
        screen->panels[1].flags[0] = 1;
    }
    if (bits & 8) {
        screen->panels[1].flags[1] = 1;
    }
    if (bits & 0x20) {
        screen->panels[1].flags[2] = 1;
    }
    if (bits & 0x80) {
        screen->panels[1].flags[3] = 1;
    }
    if (bits & 0x200) {
        screen->panels[1].flags[4] = 1;
    }
    if (bits & 0x800) {
        screen->panels[1].flags[5] = 1;
    }
    if (bits & 0x2000) {
        screen->panels[1].flags[6] = 1;
    }
    if (bits & 0x8000) {
        screen->panels[1].flags[7] = 1;
    }
    if (bits & 0x20000) {
        screen->panels[1].flags[8] = 1;
    }
    if (bits & 0x80000) {
        screen->panels[1].flags[9] = 1;
    }
}

/* CardScreen.clearPanelFlags: clears both panels' blink flags */
void CARDGAME_clearPanelFlags(CardScreen *screen) {
    HEAP.zero(screen->panels[0].flags, sizeof(screen->panels[0].flags));
    HEAP.zero(screen->panels[1].flags, sizeof(screen->panels[1].flags));
}

/* CardScreen.removeSprite: hides a sprite */
s32 CARDGAME_removeSprite(CardScreen *screen, s32 index) {
    screen->sprites[index].state = 0;
    screen->sprites[index].visible = 0;
    screen->sprites[index].index = 0;
    return 0;
}

/* Starts a sprite's blink (state 11), with the confirm sound */
s32 CARDGAME_startSpriteBlink(CardScreen *screen, s32 index) {
    SOUND.playSound(SOUND_MENU_CONFIRM);
    screen->sprites[index].state = 11;
    screen->sprites[index].duration = 0;
    screen->sprites[index].time = 0;
    return 0;
}

/* CardScreen.dealSprites: lays out count sprites as a hand from x, y, at
   once (duration 0) or moving there */
void CARDGAME_dealSprites(CardScreen *screen, s16 duration, s16 count, s32 x, s32 y) {
    s32 i;
    s32 sx;

    for (i = 0; i < count; i++) {
        sx = x + CARDGAME_getHandOffset(count, i);
        if (duration == 0) {
            CARDGAME_addSprite(screen, i, sx, y);
        } else {
            CARDGAME_startSpriteMove(screen, i, duration, sx, y);
        }
    }
}

/* CardScreen.getCardColor: the colour of the screen's card index */
s32 CARDGAME_getCardColor(CardScreen *screen, s32 index) {
    CardDrawer drawer;

    initCardDrawer(&drawer);
    drawer.setCard(screen->cards[index] + 1);
    return drawer.card->color;
}

/* CardScreen.setSpriteCard: shows the screen's card index on sprite, with
   its AP, HP, points and colour */
void CARDGAME_setSpriteCard(CardScreen *screen, s32 sprite, s32 index) {
    CardDrawer drawer;

    initCardDrawer(&drawer);
    drawer.setCard(screen->cards[index] + 1);
    screen->sprites[sprite].index = index;
    screen->sprites[sprite].ap = drawer.card->ap;
    screen->sprites[sprite].hp = drawer.card->hp;
    screen->sprites[sprite].points = drawer.card->points;
    screen->sprites[sprite].color = drawer.card->color - 1;
    if (drawer.card->kind == 0x10) {
        screen->sprites[sprite].isKind16 = 1;
    } else {
        screen->sprites[sprite].isKind16 = 0;
    }
}

/* Loads a card's image to VRAM slot slot, in columns of eight, with its CLUT */
void CARDGAME_loadCardImage(TimLoader *loader, u8 *image, s32 slot) {
    loader->setImagePos(0x140 + slot / 8 * 16, 0x100 + slot % 8 * 32);
    loader->setClutPos(0x300, 0x100 + slot);
    loader->load(image);
}

/* Card index of CARDGAME_otherCards */
s32 CARDGAME_getOtherCard(s32 index) {
    return CARDGAME_otherCards[index];
}

/* CardScreen.loadCardImages: loads the images of the player's 40 cards, the
   opponent's 40, the 9 common ones and the 100 others, and lists them in
   dst; returns how many */
s32 CARDGAME_loadCardImages(s16 *dst, s16 *player, s16 *opponent) {
    TimLoader loader;
    CardDrawer drawer;
    s32 i;
    s32 count = 0;

    initCardDrawer(&drawer);
    initTimLoader(&loader);
    for (i = 0; i < 40; i++) {
        count++;
        drawer.setCard(player[i] + 1);
        CARDGAME_loadCardImage(&loader, drawer.card->tim, i);
        dst[i] = player[i];
    }
    for (i = 0; i < 40; i++) {
        count++;
        drawer.setCard(opponent[i] + 1);
        CARDGAME_loadCardImage(&loader, drawer.card->tim, i + 40);
        dst[i + 40] = opponent[i];
    }
    for (i = 0; i < 9; i++) {
        count++;
        drawer.setCard(CARDGAME_commonCards[i] + 1);
        CARDGAME_loadCardImage(&loader, drawer.card->tim, i + 80);
        dst[i + 80] = CARDGAME_commonCards[i];
    }
    for (i = 0; i < 100; i++) {
        drawer.setCard(CARDGAME_getOtherCard(i) + 1);
        count++;
        CARDGAME_loadCardImage(&loader, drawer.card->tim, i + 89);
        dst[i + 89] = CARDGAME_getOtherCard(i);
    }
    return count;
}

/* Creates the card screen over cards, with its functions */
CardScreen *CARDGAME_createScreen(s16 *cards) {
    CardScreen *screen = createTask(CARDGAME_updateScreen, sizeof(CardScreen), 14 * 4);

    screen->setPanelValue = CARDGAME_setPanelValue;
    screen->openGauge = CARDGAME_openGauge;
    screen->closeGauge = CARDGAME_closeGauge;
    screen->openWindow = CARDGAME_openWindow;
    screen->closeWindow = CARDGAME_closeWindow;
    screen->setPanelFlags = CARDGAME_setPanelFlags;
    screen->clearPanelFlags = CARDGAME_clearPanelFlags;
    screen->openMessage = CARDGAME_openMessage;
    screen->closeMessage = CARDGAME_closeMessage;
    screen->setMessageChoice = CARDGAME_setMessageChoice;
    screen->confirmMessage = CARDGAME_confirmMessage;
    screen->openMenu = CARDGAME_openMenuWindow;
    screen->closeMenu = CARDGAME_closeMenuWindow;
    screen->confirmMenu = CARDGAME_confirmMenuWindow;
    screen->setMenuRow = CARDGAME_setMenuWindowRow;
    screen->getHandOffset = CARDGAME_getHandOffset;
    screen->showBlinker = CARDGAME_showBlinker;
    screen->startBlinkerMove = CARDGAME_startBlinkerMove;
    screen->startMove = CARDGAME_startSpriteMove;
    screen->startSlide = CARDGAME_startSpriteSlide;
    screen->startFly = CARDGAME_startSpriteFly;
    screen->startJitter = CARDGAME_startSpriteJitter;
    screen->startShake = CARDGAME_startSpriteShake;
    screen->startRecovery = CARDGAME_startSpriteRecovery;
    screen->startEffect = CARDGAME_startSpriteEffect;
    screen->startFlip = CARDGAME_startSpriteFlip;
    screen->addSprite = CARDGAME_addSprite;
    screen->removeSprite = CARDGAME_removeSprite;
    screen->startBlink = CARDGAME_startSpriteBlink;
    screen->setSpriteScale = CARDGAME_setSpriteScale;
    screen->scaleSprite = CARDGAME_scaleSprite;
    screen->dealSprites = CARDGAME_dealSprites;
    screen->cards = cards;
    screen->closePanels = CARDGAME_closePanels;
    screen->openPanels = CARDGAME_openPanels;
    screen->closePanel = CARDGAME_closePanel;
    screen->openPanelIcon = CARDGAME_openPanelIcon;
    screen->closePanelIcon = CARDGAME_closePanelIcon;
    screen->openPanel = CARDGAME_openPanel;
    screen->resetPanels = CARDGAME_resetPanels;
    screen->setSpriteCard = CARDGAME_setSpriteCard;
    screen->getCardColor = CARDGAME_getCardColor;
    screen->loadCardImages = CARDGAME_loadCardImages;
    return screen;
}
