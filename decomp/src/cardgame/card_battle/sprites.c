/* CARDGAME's card sprites: their moves, effects and drawing. */

#include "cardgame/card_battle.h"

/* Whether sprite is scaled: its scaleX and scaleY, read as one word, aren't
   both 0x1000 */
static inline s32 isScaled(CardSprite *sprite) {
    return *(s32 *)&sprite->scaleX != 0x10001000;
}

/* Moves and scales a sprite towards its targets; at the end it plays a sound (unless in state 3) and goes to state 1 */
void CARDGAME_moveSprite(CardScreen *screen, CardSprite *sprite) {
    sprite->time -= GFX.funcs.getFrameTime();
    if (sprite->time > 0) {
        if (sprite->x != sprite->targetX || sprite->y != sprite->targetY) {
            sprite->x = sprite->targetX - (sprite->targetX - sprite->startX) * sprite->time / sprite->duration;
            sprite->y = sprite->targetY - (sprite->targetY - sprite->startY) * sprite->time / sprite->duration;
        }
        /* both scales, read as one word */
        if (*(s32 *)&sprite->scaleX != *(s32 *)&sprite->targetScaleX) {
            sprite->scaleX = sprite->targetScaleX - (sprite->targetScaleX - sprite->startScaleX) * sprite->time / sprite->duration;
            sprite->scaleY = sprite->targetScaleY - (sprite->targetScaleY - sprite->startScaleY) * sprite->time / sprite->duration;
        }
    } else {
        if (sprite->state != 3) {
            SOUND.playSound(SOUND_CARD_001);
        }
        sprite->state = 1;
        sprite->time = 0;
        sprite->duration = 0;
        sprite->x = sprite->targetX;
        sprite->y = sprite->targetY;
        sprite->scaleX = sprite->targetScaleX;
        sprite->scaleY = sprite->targetScaleY;
        if (sprite->highlight == 0) {
            sprite->moving = 0;
        }
    }
}

/* Flies a sprite from start to target along an arc while scaling it; at the end it swaps start and target, goes to state 2 with 2.5 times the time, and returns 1 */
s32 CARDGAME_flySprite(CardScreen *screen, CardSprite *sprite) {
    s32 done = 0;
    s32 dy;
    s32 offset;
    s32 startX;
    s32 startY;
    s32 scaledDy;
    s16 scaleX;
    s16 scaleY;

    if (sprite->growTime > 0) {
        sprite->growTime -= GFX.funcs.getFrameTime();
        sprite->scaleX = 0x1C00 - sprite->growTime * 0xC0;
        sprite->scaleY = 0x1C00 - sprite->growTime * 0xC0;
        if (sprite->growTime <= 0) {
            sprite->highlight |= 5;
            SOUND.playSound(0x8004603C);
            sprite->targetScaleX = 0x1200;
            sprite->targetScaleY = 0x1200;
            sprite->startScaleX = sprite->scaleX = 0x1C00;
            sprite->startScaleY = sprite->scaleY = 0x1C00;
            sprite->growTime = 0;
        }
        return 0;
    }
    sprite->time -= GFX.funcs.getFrameTime();
    if (sprite->time > 0) {
        dy = sprite->targetY - sprite->startY;
        scaledDy = dy * sprite->time;
        sprite->x = sprite->targetX - (sprite->targetX - sprite->startX) * sprite->time / sprite->duration;
        sprite->y = sprite->targetY - scaledDy / sprite->duration;
        offset = rsin((sprite->time << 12) / (sprite->duration * 2)) * 0x1C00;
        sprite->y += (dy > 0 ? -offset : offset) / 0x1000;
        sprite->scaleX = sprite->targetScaleX - (sprite->targetScaleX - sprite->startScaleX) * sprite->time / sprite->duration;
        sprite->scaleY = sprite->targetScaleY - (sprite->targetScaleY - sprite->startScaleY) * sprite->time / sprite->duration;
    } else {
        startX = sprite->startX;
        startY = sprite->startY;
        sprite->x = sprite->startX = sprite->targetX;
        sprite->y = sprite->startY = sprite->targetY;
        sprite->targetX = startX;
        sprite->targetY = startY;
        scaleX = sprite->targetScaleX;
        scaleY = sprite->targetScaleY;
        sprite->state = 2;
        sprite->targetScaleX = 0x1000;
        sprite->targetScaleY = 0x1000;
        sprite->scaleX = scaleX;
        sprite->scaleY = scaleY;
        sprite->highlight &= ~5;
        sprite->time = sprite->duration = sprite->duration * 2 + sprite->duration / 2;
        sprite->startScaleX = sprite->scaleX;
        sprite->startScaleY = sprite->scaleY;
        done = 1;
    }
    return done;
}

/* Shakes a sprite sideways and stretches it, for 12 frames: 1 at the end */
s32 CARDGAME_shakeSprite(CardScreen *screen, CardSprite *sprite) {
    s32 done = 0;

    sprite->x = sprite->startX + CARDGAME_shakeOffsets[sprite->time & 7];
    sprite->scaleY = rsin((sprite->time << 12) / 24) / 16 + 0x1000;
    sprite->time += GFX.funcs.getFrameTime();
    if (sprite->time >= 12) {
        sprite->state = 1;
        sprite->moving = 0;
        sprite->effect = 0;
        sprite->scaleY = 0x1000;
        sprite->x = sprite->startX;
        done = 1;
    }
    return done;
}

/* Ends a sprite's move and puts it in STATE */
static inline void resetSprite(CardSprite *sprite, s32 state) {
    sprite->state = state;
    sprite->duration = 0;
    sprite->time = 0;
    sprite->moving = 0;
    sprite->effect = 0;
}

/* Holds a sprite for 10 frames, then starts its blink: 1 at the end */
s32 CARDGAME_holdSpriteThenBlink(CardScreen *screen, CardSprite *sprite) {
    s32 done = 0;

    sprite->time += GFX.funcs.getFrameTime();
    if (sprite->time >= 10) {
        resetSprite(sprite, 11);
        done = 1;
    }
    return done;
}

/* Holds a sprite for duration frames, then ends its effect: 1 at the end */
s32 CARDGAME_holdSprite(CardScreen *screen, CardSprite *sprite, s32 duration) {
    s32 done = 0;

    sprite->time += GFX.funcs.getFrameTime();
    if (sprite->time >= duration) {
        resetSprite(sprite, 1);
        done = 1;
    }
    return done;
}

/* Jitters a sprite about where it started and squashes it, for 12 frames: 1
   at the end */
s32 CARDGAME_jitterSprite(CardScreen *screen, CardSprite *sprite) {
    s32 done;

    sprite->x = sprite->startX + CARDGAME_jitterOffsets[sprite->time & 7];
    sprite->y = sprite->startY + CARDGAME_jitterOffsets[RANDOM.next() & 7];
    done = 0;
    sprite->scaleY = 0x1000 - rsin((sprite->time << 12) / 24) / 8;
    sprite->time += GFX.funcs.getFrameTime();
    if (sprite->time >= 12) {
        sprite->state = 1;
        sprite->moving = 0;
        sprite->scaleY = 0x1000;
        sprite->effect = 0;
        sprite->x = sprite->startX;
        sprite->y = sprite->startY;
        done = 1;
    }
    return done;
}

/* The end of a sprite's flip (CARDGAME_flipSprite) */
static inline void endSpriteFlip(CardSprite *sprite, s32 state) {
    sprite->state = state;
    sprite->moving = 0;
    sprite->scaleX = 0x1000;
}

/* Turns a sprite over for 12 frames, its side swapped after 7: 1 at the end */
s32 CARDGAME_flipSprite(CardScreen *screen, CardSprite *sprite) {
    s32 done = 0;

    sprite->scaleX = 0x1000 - rsin((sprite->time << 12) / 24);
    sprite->time += GFX.funcs.getFrameTime();
    if (sprite->duration == 0 && sprite->time >= 7) {
        sprite->duration = 1;
        sprite->visible ^= 3;
    }
    if (sprite->time >= 12) {
        endSpriteFlip(sprite, 1);
        done = 1;
    }
    return done;
}

/* The part of a sprite sheet at offset, one of the offsets it starts with */
static inline void *getSheetPart(s32 *sheet, s32 offset) {
    return (u8 *)sheet + offset;
}

/* Draws a card's picture: a one-part sprite sheet (CARDGAME_pictureSheet) made on the fly,
   the 32x32 cell `index` of an 8-row grid with the card's own CLUT row */
void CARDGAME_drawCardPicture(CardSprite *sprite) {
    SpriteDrawer drawer;
    s16 *frame;
    SpritePart *part;
    s32 u;
    s32 v;
    s32 *sheet;

    u = sprite->index / 8 * 32;
    v = sprite->index % 8 * 32;
    sheet = CARDGAME_pictureSheet;
    frame = getSheetPart(sheet, sheet[2]);
    frame[0] = 1;
    frame[2] = -1;
    frame[1] = sprite->index;
    frame[3] = 0;
    frame[4] = 4;
    frame[5] = 2;
    part = getSheetPart(sheet, sheet[0]);
    part->u = u;
    part->v = v;
    part->w = 32;
    part->h = 32;
    part->clutX = 0;
    part->clutY = 0x100;
    part->mode = 1;
    initSpriteDrawer(&drawer);
    if (isScaled(sprite)) {
        drawer.setPivot((sprite->x >> 8) + 0x14, (sprite->y >> 8) + 0x17);
        drawer.setScale(sprite->scaleX, sprite->scaleY, 0x1000);
    }
    drawer.setLayerId(0x100, 1);
    drawer.setTexture(0x140, 0x100);
    drawer.setAltClut(0x300, 0x100);
    drawer.draw(sheet, 0, sprite->x >> 8, sprite->y >> 8);
}

/* Draws a sprite's effect animation (1-4, frames 0x28-0x4C of the fourth TIM) and ends it when its time runs out */
void CARDGAME_drawSpriteEffect(CardScreen *screen, CardSprite *sprite) {
    SpriteDrawer drawer;

    if (sprite->visible != 0 && sprite->effect != 0) {
        initSpriteDrawer(&drawer);
        /* both scales at 0x1000, read as one word */
        if (isScaled(sprite)) {
            drawer.setPivot((sprite->x >> 8) + 0x14, (sprite->y >> 8) + 0x17);
            drawer.setScale(sprite->scaleX, sprite->scaleY, 0x1000);
        }
        drawer.setLayerId(0x100, 1);
        drawer.setTexture(0x340, 0);
        switch (sprite->effect) {
        case 1:
            drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), CARDGAME_loopFrames[(screen->time >> 2) & 3] + 0x28,
                        sprite->x >> 8, sprite->y >> 8);
            break;
        case 2:
            drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), CARDGAME_onceFrames[(sprite->effectTime >> 2) % 4] + 0x2C,
                        sprite->x >> 8, sprite->y >> 8);
            if (sprite->effectTime >= 8) {
                sprite->effect = 0;
                sprite->effectTime = 0;
            }
            sprite->effectTime += GFX.funcs.getFrameTime();
            break;
        case 3:
            drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), (sprite->effectTime >> 1) % 9 + 0x3C,
                        sprite->x >> 8, sprite->y >> 8);
            if (sprite->effectTime++ >= 36) {
                sprite->effect = 0;
                sprite->effectTime = 0;
            }
            sprite->effectTime += GFX.funcs.getFrameTime();
            break;
        case 4:
            drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), (sprite->effectTime >> 1) % 7 + 0x46,
                        sprite->x >> 8, sprite->y >> 8);
            if (sprite->effectTime >= 28) {
                sprite->effect = 0;
                sprite->effectTime = 0;
            }
            sprite->effectTime += GFX.funcs.getFrameTime();
            break;
        }
    }
}

/* Draws the highlights over a sprite (highlight bits 0-2), with a cycling palette */
void CARDGAME_drawSpriteHighlights(CardScreen *screen, CardSprite *sprite) {
    SpriteDrawer drawer;

    if (sprite->visible != 0) {
        if (sprite->highlight & 6) {
            initSpriteDrawer(&drawer);
            if (sprite->highlight & 4) {
                drawer.setClutRow(CARDGAME_highlightCluts[(screen->time >> 2) % 6] + 5);
            } else {
                drawer.setClutRow(4);
            }
            /* both scales at 0x1000, read as one word */
            if (isScaled(sprite)) {
                drawer.setPivot((sprite->x >> 8) + 0x14, (sprite->y >> 8) + 0x17);
                drawer.setScale(sprite->scaleX, sprite->scaleY, 0x1000);
            }
            drawer.setLayerId(0x100, 1);
            drawer.setTexture(0x340, 0);
            drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), 8, sprite->x >> 8, sprite->y >> 8);
        }
        if (sprite->highlight & 1) {
            initSpriteDrawer(&drawer);
            drawer.setClutRow(CARDGAME_highlightCluts[(screen->time >> 2) % 6]);
            /* both scales at 0x1000, read as one word */
            if (isScaled(sprite)) {
                drawer.setPivot((sprite->x >> 8) + 0x14, (sprite->y >> 8) + 0x17);
                drawer.setScale(sprite->scaleX, sprite->scaleY, 0x1000);
            }
            drawer.setLayerId(0x100, 1);
            drawer.setTexture(0x340, 0);
            drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), 8, sprite->x >> 8, sprite->y >> 8);
        }
    }
}

/* Draws a sprite's order number (order), the place of its card in the record */
void CARDGAME_drawSpriteOrder(CardScreen *screen, CardSprite *sprite) {
    SpriteDrawer drawer;

    if (sprite->visible != 0 && sprite->order != 0) {
        initSpriteDrawer(&drawer);
        /* both scales at 0x1000, read as one word */
        if (isScaled(sprite)) {
            drawer.setPivot((sprite->x >> 8) + 0x14, (sprite->y >> 8) + 0x17);
            drawer.setScale(sprite->scaleX, sprite->scaleY, 0x1000);
        }
        drawer.setLayerId(0x100, 1);
        drawer.setTexture(0x280, 0);
        drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 2), sprite->order + 0x33, (sprite->x >> 8) + 3, (sprite->y >> 8) + 2);
    }
}

/* Draws a card's marks that are set (marks[0..2]: sprites 0x37 to 0x39), in a row 8 pixels apart */
void CARDGAME_drawSpriteMarks(CardScreen *screen, CardSprite *sprite) {
    SpriteDrawer drawer;
    s32 i;
    s32 n;

    if (sprite->visible != 0) {
        for (i = 0, n = 0; i < 3; i++) {
            if (sprite->marks[i] != 0) {
                initSpriteDrawer(&drawer);
                /* both scales at 0x1000, read as one word */
                if (isScaled(sprite)) {
                    drawer.setPivot((sprite->x >> 8) + 0x14, (sprite->y >> 8) + 0x17);
                    drawer.setScale(sprite->scaleX, sprite->scaleY, 0x1000);
                }
                drawer.setLayerId(0x100, 1);
                drawer.setTexture(0x280, 0);
                drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 2), i + 0x37, (sprite->x >> 8) + 3 + n * 8, (sprite->y >> 8) + 0x15);
                n++;
            }
        }
    }
}

/* Draws a card's two numbers (ap and hp) and the sprite between them */
static inline void drawCardNumbers(CardSprite *sprite) {
    CardNumber number;
    SpriteDrawer drawer;

    number.depth = 1;
    number.digits = 2;
    number.leadingZeros = 0;
    number.x = (sprite->x >> 8) + 4;
    number.y = (sprite->y >> 8) + 0x21;
    number.value = sprite->ap;
    number.pivotX = (sprite->x >> 8) + 0x14;
    number.pivotY = (sprite->y >> 8) + 0x17;
    number.scaleX = sprite->scaleX;
    number.scaleY = sprite->scaleY;
    CARDGAME_drawNumber(&number, 1);
    number.x = (sprite->x >> 8) + 0x17;
    number.y = (sprite->y >> 8) + 0x21;
    number.value = sprite->hp;
    number.pivotX = (sprite->x >> 8) + 0x14;
    number.pivotY = (sprite->y >> 8) + 0x17;
    number.scaleX = sprite->scaleX;
    number.scaleY = sprite->scaleY;
    CARDGAME_drawNumber(&number, 1);
    initSpriteDrawer(&drawer);
    /* both scales at 0x1000, read as one word */
    if (isScaled(sprite)) {
        drawer.setPivot((sprite->x >> 8) + 0x14, (sprite->y >> 8) + 0x17);
        drawer.setScale(sprite->scaleX, sprite->scaleY, 0x1000);
    }
    drawer.setLayerId(0x100, 1);
    drawer.setTexture(0x340, 0);
    drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), 0x1F, (sprite->x >> 8) + 0x12, (sprite->y >> 8) + 0x21);
}

/* Draws the mark (sprite 0x1E) of a card that is not of kind 16 */
static inline void drawCardMark(CardSprite *sprite) {
    SpriteDrawer drawer;

    initSpriteDrawer(&drawer);
    /* both scales at 0x1000, read as one word */
    if (isScaled(sprite)) {
        drawer.setPivot((sprite->x >> 8) + 0x14, (sprite->y >> 8) + 0x17);
        drawer.setScale(sprite->scaleX, sprite->scaleY, 0x1000);
    }
    drawer.setLayerId(0x100, 1);
    drawer.setTexture(0x340, 0);
    drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), 0x1E, (sprite->x >> 8) + 4, (sprite->y >> 8) + 0x21);
}

/* Draws a card sprite: its frame (visible 1, with the numbers or the mark and the image) or its back (visible 2 or 3) */
void CARDGAME_drawSpriteCard(CardScreen *screen, CardSprite *sprite) {
    /* the match depends on the early return together with the empty case 0 */
    if (sprite->visible == 0) {
        return;
    }
    switch (sprite->visible) {
    case 0:
        break;
    case 1:
        if (sprite->isKind16 != 0) {
            drawCardNumbers(sprite);
        } else {
            drawCardMark(sprite);
        }
        {
            SpriteDrawer drawer;

            initSpriteDrawer(&drawer);
            /* both scales at 0x1000, read as one word */
            if (isScaled(sprite)) {
                drawer.setPivot((sprite->x >> 8) + 0x14, (sprite->y >> 8) + 0x17);
                drawer.setScale(sprite->scaleX, sprite->scaleY, 0x1000);
            }
            drawer.setLayerId(0x100, 1);
            drawer.setTexture(0x280, 0);
            drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 2), sprite->dimmed == 1 ? sprite->color + 0x3C : sprite->color,
                        sprite->x >> 8, sprite->y >> 8);
        }
        CARDGAME_drawCardPicture(sprite);
        break;
    case 2: {
        SpriteDrawer drawer;

        initSpriteDrawer(&drawer);
        /* both scales at 0x1000, read as one word */
        if (isScaled(sprite)) {
            drawer.setPivot((sprite->x >> 8) + 0x14, (sprite->y >> 8) + 0x17);
            drawer.setScale(sprite->scaleX, sprite->scaleY, 0x1000);
        }
        drawer.setLayerId(0x100, 1);
        drawer.setTexture(0x280, 0);
        drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 2), 0x10, sprite->x >> 8, sprite->y >> 8);
        break;
    }
    case 3: {
        SpriteDrawer drawer;

        initSpriteDrawer(&drawer);
        /* both scales at 0x1000, read as one word */
        if (isScaled(sprite)) {
            drawer.setPivot((sprite->x >> 8) + 0x14, (sprite->y >> 8) + 0x17);
            drawer.setScale(sprite->scaleX, sprite->scaleY, 0x1000);
        }
        drawer.setLayerId(0x100, 1);
        drawer.setTexture(0x280, 0);
        drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 2), 0xC, sprite->x >> 8, sprite->y >> 8);
        break;
    }
    }
}

/* Draws the shade over a dimmed sprite */
void CARDGAME_drawSpriteDim(CardScreen *screen, CardSprite *sprite) {
    SpriteDrawer drawer;

    if (sprite->visible != 0 && sprite->dimmed != 0) {
        initSpriteDrawer(&drawer);
        /* both scales at 0x1000, read as one word */
        if (isScaled(sprite)) {
            drawer.setPivot((sprite->x >> 8) + 0x14, (sprite->y >> 8) + 0x17);
            drawer.setScale(sprite->scaleX, sprite->scaleY, 0x1000);
        }
        drawer.setLayerId(0x100, 1);
        drawer.setTexture(0x340, 0);
        drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), 10, sprite->x >> 8, sprite->y >> 8);
    }
}

/* Draws a blinking sprite's flash (state 11), its CLUT row changing */
void CARDGAME_drawSpriteBlink(CardScreen *screen, CardSprite *sprite) {
    SpriteDrawer drawer;

    if (sprite->visible != 0 && sprite->state == 11) {
        initSpriteDrawer(&drawer);
        drawer.setClutRow((sprite->duration / 2) % 5);
        drawer.setPivot((sprite->x >> 8) + 0x14, (sprite->y >> 8) + 0x17);
        drawer.setScale(sprite->scaleX, sprite->scaleY, 0x1000);
        drawer.setLayerId(0x100, 1);
        drawer.setTexture(0x340, 0);
        drawer.draw(FILE_CACHE.getEntry(FILE_CARDGAME_TIMS << 16 | 3), 9, sprite->x >> 8, sprite->y >> 8);
    }
}

/* Runs a sprite's animation for its state; sets bit 0 of spriteFlags while one runs */
void CARDGAME_animateSprite(CardScreen *screen, CardScreenItems *items, CardSprite *sprite) {
    switch (sprite->state) {
    case 0:
        sprite->visible = 0;
        break;
    case 2:
    case 3:
        screen->spriteFlags |= 1;
        CARDGAME_moveSprite(screen, sprite);
        break;
    case 4:
        screen->spriteFlags |= 1;
        CARDGAME_flipSprite(screen, sprite);
        break;
    case 5:
        screen->spriteFlags |= 1;
        if (CARDGAME_flySprite(screen, sprite) != 0) {
            screen->spriteFlags |= 2;
        }
        break;
    case 6:
        screen->spriteFlags |= 1;
        CARDGAME_jitterSprite(screen, sprite);
        break;
    case 7:
        screen->spriteFlags |= 1;
        CARDGAME_shakeSprite(screen, sprite);
        break;
    case 8:
        screen->spriteFlags |= 1;
        CARDGAME_holdSpriteThenBlink(screen, sprite);
        break;
    case 11:
        screen->spriteFlags |= 1;
        sprite->duration += GFX.funcs.getFrameTime();
        if (sprite->duration >= 11) {
            sprite->state = 1;
        }
        break;
    case 9:
        CARDGAME_holdSprite(screen, sprite, 36);
        screen->spriteFlags |= 1;
        break;
    case 10:
        CARDGAME_holdSprite(screen, sprite, 28);
        screen->spriteFlags |= 1;
        break;
    case 1:
        break;
    }
}

/* Draws a sprite and its marks, unless it is scaled to nothing */
void CARDGAME_drawSprite(CardScreen *screen, CardSprite *sprite) {
    if (sprite->scaleX != 0 && sprite->scaleY != 0) {
        CARDGAME_drawSpriteHighlights(screen, sprite);
        CARDGAME_drawSpriteMarks(screen, sprite);
        CARDGAME_drawSpriteOrder(screen, sprite);
        CARDGAME_drawSpriteEffect(screen, sprite);
        CARDGAME_drawSpriteDim(screen, sprite);
        CARDGAME_drawSpriteCard(screen, sprite);
    }
}

/* Animates the sprites, then draws them: the moving ones first */
void CARDGAME_updateSprites(CardScreen *screen, CardScreenItems *items) {
    s32 i;
    s32 pass;

    for (i = 39; i >= 0; i--) {
        CARDGAME_animateSprite(screen, items, &screen->sprites[i]);
        CARDGAME_drawSpriteBlink(screen, &screen->sprites[i]);
    }
    for (pass = 0; pass < 2; pass++) {
        for (i = 39; i >= 0; i--) {
            if ((pass == 0 && screen->sprites[i].moving != 0) || (pass != 0 && screen->sprites[i].moving == 0)) {
                CARDGAME_drawSprite(screen, &screen->sprites[i]);
            }
        }
    }
}
