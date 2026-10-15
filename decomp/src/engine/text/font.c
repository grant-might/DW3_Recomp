#include "engine/game.h"

/* Loads the font's images into VRAM and starts loading the menu sprites */
void loadFont(void) {
    TimLoader obj;

    initTimLoader(&obj);
    obj.setImagePos(0x140, 0);
    obj.loadArchive(FILE_CACHE.getEntry(FILE_FONT << 16));
    FILE_CACHE.free(FILE_FONT);
    FILE_CACHE.request(FILE_MENU_SPRITES);
}


/* The next character as kind << 8 | value (see Font); `mode` set for Shift-JIS text */
s16 decodeChar(u8 *s, u8 mode, TextStyle *font) {
    u16 code;
    GlyphMap *map;
    s32 i;

    if (s[0] == 0) {
        return 0x400;
    }
    if (mode != 0) {
        if (s[0] < 4) {
            return (s[0] << 8) | s[1];
        }
        if (s[0] == 0xA) {
            return 0x201;
        }
        code = s[0] << 8;
        code |= s[1];
        if ((u16)(code - 0x824F) < 0x146) {
            map = font->sjisMap;
            for (i = 4; map[i].code != 0xFFFF; i++) {
                if (code == map[i].code) {
                    return map[i].index;
                }
            }
        } else {
            map = font->iconMap;
            for (i = 0; map[i].code != 0xFFFF; i++) {
                if (code == map[i].code) {
                    return map[i].index | 0x100;
                }
            }
        }
    } else {
        if (s[0] == 1) {
            if (s[1] > font->iconCount) {
                return (((u16)font->iconCount + 1) & 0xFF) | 0x100;
            }
            return (s[0] << 8) | s[1];
        }
        if (s[0] < 4) {
            return (s[0] << 8) | s[1];
        }
        if (s[0] < font->glyphCount) {
            return s[0];
        }
    }
    return 0x300;
}

/* Style 0 is unused; the others have lines of 14, 11 and 9 pixels */
TextStyle FONT_STYLES[4] = {
    { 0xFF, 0, 0, 0, NULL, NULL, 0, 0 },
    { 0xFF, 14, FONT_GLYPHS_1, FONT_ICONS_1, FONT_GLYPH_MAP, FONT_ICON_MAP, 234, 114 },
    { 0xFF, 11, FONT_GLYPHS_2, FONT_ICONS_2, FONT_GLYPH_MAP, FONT_ICON_MAP, 234, 114 },
    { 0xFF, 9, FONT_GLYPHS_3, FONT_ICONS_3, FONT_GLYPH_MAP, FONT_ICON_MAP, 234, 114 },
};

/* The length of each control code, with its arguments */
s32 FONT_CODE_LENGTHS[11] = { 1, 2, 3, 2, 5, 3, 3, 2, 3, 2, 0 };

/* decode is declared without a prototype, as callers would truncate the
   u8 mode, so decodeChar is cast to it */
Font FONT = { FONT_STYLES, FONT_CODE_LENGTHS, loadFont, (s16 (*)())decodeChar };
