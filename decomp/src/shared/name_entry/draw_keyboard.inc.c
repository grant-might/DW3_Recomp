/* Draws the keyboard, the cursor, the partner and the name entry's panels */
void OVL_NAME(drawKeyboard)(NameEntry *task) {
    SpriteDrawer sprite;
    s32 i;
    s32 key;

    initSpriteDrawer(&sprite);
    sprite.setTexture(task->vramX, task->vramY);
    sprite.setLayerId(task->layer, task->depth);
    if (task->keyboardScale.level != 0) {
        if (task->active) {
            if (GFX.funcs.getTime() - task->keyTime >= 5) {
                task->keyTime = GFX.funcs.getTime();
                if (++task->keyFrame >= 4) {
                    task->keyFrame = 0;
                }
            }
            sprite.setClutRow(task->keyFrame);
            if (task->column < 10 || task->row < 3) {
                sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), 0x27,
                            task->column * 14 + 0x2F + task->column / 5 * 8, task->row * 18 + 0x5A);
            } else {
                for (i = 0; OVL_NAME(keyboard).pages[task->page].cells[task->row][task->column + i].kind != 1; i--) {
                }
                switch (task->column + i + task->row * 15) {
                case NAME_KEY_LEFT:
                default:
                    key = 0;
                    break;
                case NAME_KEY_RIGHT:
                    key = 1;
                    break;
                case NAME_KEY_DELETE:
                    key = 2;
                    OVL_NAME(bigKeys)[2].sprite = NAME_ENTRY_TEXT_SPRITE(0x3C);
                    break;
                case NAME_KEY_SPACE:
                    key = 3;
                    OVL_NAME(bigKeys)[3].sprite = NAME_ENTRY_TEXT_SPRITE(0x44);
                    break;
                case NAME_KEY_END:
                    key = 4;
                    OVL_NAME(bigKeys)[4].sprite = NAME_ENTRY_TEXT_SPRITE(0x4C);
                    break;
                }
                sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), OVL_NAME(bigKeys)[key].sprite,
                            OVL_NAME(bigKeys)[key].x, OVL_NAME(bigKeys)[key].y);
            }
            sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), 0x28, task->cursor * 19 + 0x4B, 0x40);
            sprite.setClutRow(0);
        }
        if (task->keyboardScale.level != ONE) {
            sprite.setScale(task->keyboardScale.level, ONE, ONE);
        }
        if (task->keyboardScale.level != ONE) {
            sprite.setPivot(0x18, 0x20);
        }
        sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), 0x1D, 0x18, 0x15);
        if (task->mode != 2) {
            if (task->keyboardScale.level != ONE) {
                sprite.setPivot(0x20, 0x3F);
            }
            if (task->partner != -1) {
                if (GFX.funcs.getTime() - task->partnerTime >= 13) {
                    task->partnerTime = GFX.funcs.getTime();
                    if (++task->partnerFrame >= 7 ||
                        OVL_NAME(nameAnims)[task->partner][task->partnerFrame] == -1) {
                        task->partnerFrame = 0;
                    }
                }
                sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES),
                            OVL_NAME(nameAnims)[task->partner][task->partnerFrame], 0x22, 0x30);
            } else {
                sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), 0x36, 0x20, 0x2E);
            }
            if (GFX.funcs.getTime() - task->clutTime >= 5) {
                task->clutTime = GFX.funcs.getTime();
                if (++task->clutRow >= 14) {
                    task->clutRow = 0;
                }
            }
            sprite.setLayerId(task->layer, task->depth - 1);
            sprite.setClutRow(task->clutRow);
            sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), 0x1F, 0x20, 0x2E);
            sprite.setClutRow(0);
            sprite.setLayerId(task->layer, task->depth);
            sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), 0x1E, 0x20, 0x2E);
        }
        if (task->keyboardScale.level != ONE) {
            sprite.setPivot(0x20, 0x49);
        }
        if (task->mode != 2) {
            if (NAME_ENTRY_JAPANESE) {
                sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), 0x34, 0x4B, 0x40);
            } else {
                sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), 0x37, 0x4B, 0x40);
            }
        } else {
            sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), 0x35, 0x4B, 0x40);
        }
        if (task->keyboardScale.level != ONE) {
            sprite.setScale(ONE, task->keyboardScale.level, ONE);
        }
        if (OVL_NAME(keyboard).pageCount >= 2) {
            if (GFX.funcs.getTime() - task->arrowTime >= 7) {
                task->arrowTime = GFX.funcs.getTime();
                if (++task->arrowFrame >= 6) {
                    task->arrowFrame = 0;
                }
            }
            sprite.setClutRow(OVL_NAME(keyArrowCluts)[task->arrowFrame]);
            sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), 0x32, 0xA, 0x5E);
            sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), 0x33, 0x119, 0x5E);
            sprite.setClutRow(0);
        }
        sprite.setClutRow(4);
        sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), 0x2C, 0xCB, 0xC3);
        sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), 0x2D, 0xDE, 0xC3);
        sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), NAME_ENTRY_TEXT_SPRITE(0x3C), 0xCB, 0x99);
        sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), NAME_ENTRY_TEXT_SPRITE(0x44), 0xCB, 0xAE);
        sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), NAME_ENTRY_TEXT_SPRITE(0x4C), 0xF6, 0xC3);
        sprite.setClutRow(0);
        sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), 0x25, 0x1D, 0x54);
    }
    sprite.setLayerId(task->layer, task->depth - 1);
    if (task->messageScale.level != 0) {
        if (task->messageScale.level != ONE) {
            sprite.setScale(ONE, task->messageScale.level, ONE);
            sprite.setPivot(0, 0x78);
        }
        sprite.draw(FILE_CACHE.getEntry(NAME_ENTRY_SPRITES), 0x26, 0, 0x64);
    }
}
