#include "engine/game.h"

/* The debug messages, in Shift-JIS */
/* "Null message was passed" */
const char STR_NULL_MESSAGE[] = "\x82\x6D\x82\x95\x82\x8C\x82\x8C\x83\x81\x83\x62\x83\x5A\x81\x5B"
                                "\x83\x57\x82\xAA\x82\xED\x82\xBD\x82\xB3\x82\xEA\x82\xDC\x82\xB5\x82\xBD";
/* "Digit: the message's work number is not supported" */
const char STR_BAD_DIGIT_BUFFER[] = "\x82\x63\x82\x89\x82\x87\x82\x89\x82\x94\x81\x46\x83\x81\x83\x62"
                                    "\x83\x5A\x81\x5B\x83\x57\x82\xCC\x83\x8F\x81\x5B\x83\x4E\x82\xCE"
                                    "\x82\xF1\x82\xB2\x82\xA4\x82\xAA\x82\xDD\x82\xBD\x82\xA2\x82\xA8\x82\xA4";
/* "ExtMess: the message's work number is not supported" */
const char STR_BAD_EXT_BUFFER[] = "\x82\x64\x82\x98\x82\x94\x82\x6C\x82\x85\x82\x93\x82\x93\x81\x46"
                                  "\x83\x81\x83\x62\x83\x5A\x81\x5B\x83\x57\x82\xCC\x83\x8F\x81\x5B"
                                  "\x83\x4E\x82\xCE\x82\xF1\x82\xB2\x82\xA4\x82\xAA\x82\xDD\x82\xBD"
                                  "\x82\xA2\x82\xA8\x82\xA4";
/* "The message is not set" */
const char STR_MESSAGE_NOT_SET[] = "\x83\x81\x83\x62\x83\x5A\x81\x5B\x83\x57\x82\xAA\x82\xB9\x82\xC1"
                                   "\x82\xC4\x82\xA2\x82\xB3\x82\xEA\x82\xC4\x82\xA2\x82\xDC\x82\xB9\x82\xF1";

/*
 * Copies a string into one of a window's text buffers, growing it as needed (NULL shows
 * STR_NULL_MESSAGE)
 */
void setTextBuffer(TextWindow *obj, TextBuffer *buf, const char *text) {
    s16 len;
    s16 cap;
    u16 size;

    if (text != NULL) {
        len = strlen(text);
        buf->len = len;
        if (len == 0) {
            obj->visible = 0;
            return;
        }
        obj->visible = 1;
        buf->sjis = 1;
        if (buf->data != NULL) {
            if (buf->cap <= buf->len) {
                HEAP.free(buf->data);
                buf->data = NULL;
                buf->cap = 0;
            }
            if (buf->data != NULL) {
                goto copy;
            }
        }
        size = buf->len;
        if (size & 3) {
            cap = (size & ~3) + 8;
        } else {
            cap = size + 4;
        }
        buf->cap = cap;
        buf->data = HEAP.alloc(cap, MEM_MODE);
    copy:
        HEAP.zero(buf->data, buf->cap);
        memcpy(buf->data, text, buf->len);
    } else {
        setTextBuffer(obj, buf, STR_NULL_MESSAGE);
    }
    if (obj->typeDelay == 0) {
        textWindowSetTypeDelay(obj, 0);
    }
}

/* Text window method: shows a string */
void textWindowSetText(TextWindow *obj, const char *text) {
    setTextBuffer(obj, &obj->text[0], text);
}

/* Text window method: shows string `id` of a string table (-1: `text` itself) */
void textWindowSetString(TextWindow *obj, char *text, s32 id) {
    textWindowSetSubString(obj, text, id, 0);
}

/* Writes a number's decimal digits (0 for zero or less) */
void formatNumber(u8 *buf, s32 value) {
    s32 saved;
    s32 len;
    s32 div;
    s32 d;

    saved = value;
    if (value <= 0) {
        *buf = '0';
        return;
    }
    len = 0;
    div = 10;
    do {
        value -= value % div;
        len++;
        div *= 10;
    } while (value != 0);
    value = saved;
    while (value != 0) {
        d = value % 10;
        value -= d;
        value /= 10;
        buf[--len] = d + '0';
    }
}

/* Text window method: puts a number into text buffer `index`, in the font's digit glyphs */
void textWindowSetNumber(TextWindow *obj, u32 index, s32 value) {
    u8 buf[16];
    u8 *p;
    s32 i;

    if (index >= 6) {
        textWindowSetText(obj, STR_BAD_DIGIT_BUFFER);
        return;
    }
    for (i = 15, p = &buf[i]; i >= 0; i--) {
        *p-- = 0;
    }
    formatNumber(buf, value);
    for (i = 0; buf[i] != 0; i++) {
        buf[i] -= 0x2C;
    }
    setTextBuffer(obj, &obj->text[index], buf);
    obj->text[index].sjis = 0;
}

/* Text window method: puts a string into work buffer `index` (1-5), for control code 5 */
void textWindowSetSubText(TextWindow *obj, const char *text, s32 index) {
    if (index < 1 || index > 5) {
        textWindowSetText(obj, STR_BAD_EXT_BUFFER);
    } else {
        setTextBuffer(obj, &obj->text[index], text);
    }
}

/* Text window method: puts string `id` of a table (-1: `text` itself) into text buffer `index` */
void textWindowSetSubString(TextWindow *obj, char *text, s32 id, s32 index) {
    TextTools cls;
    char *str;

    if (id >= 0) {
        initTextTools(&cls);
        str = cls.getString(text, id);
        if (str == NULL) {
            return;
        }
        setTextBuffer(obj, &obj->text[index], str);
    } else {
        setTextBuffer(obj, &obj->text[index], text);
    }
    obj->text[index].sjis = 0;
}

/* Text window method: draws the visible part of the text as sprites, running the control codes */
void textWindowDraw(TextWindow *obj) {
    TextDraw draw;
    SVECTOR out;
    SVECTOR in[4];
    TextBuffer *text;
    u16 clut;
    u16 prevTpage;
    s32 rotated;
    s16 c;
    s16 x;
    s16 y;
    u16 tpage;
    u8 u;
    u8 v;
    s32 i;
    s32 j;

    prevTpage = 0;
    tpage = 0;
    rotated = 0;
    if (obj->text[0].data == NULL) {
        obj->visible = 0;
        return;
    }
    for (i = 5; i >= 0; i--) {
        obj->text[i].pos = 0;
    }
    obj->finished = 0;
    obj->cursorX = -obj->alignWidth;
    draw.lineCount = 0;
    text = obj->text;
    obj->cursorY = 0;
    if (obj->scaled != 0) {
        if (obj->scale.vx == 0 && obj->scale.vy == 0) {
            return;
        }
        if (obj->scale.vx == ONE && obj->scale.vy == obj->scale.vx) {
            obj->scaled = 0;
        } else {
            rotated = 1;
            RotMatrixYXZ_gte(&obj->rot, &obj->mat);
            ScaleMatrix(&obj->mat, &obj->scale);
        }
    }
    i = 0;
    draw.layer = GFX.funcs.getLayer(obj->layerId);
    draw.ot = draw.layer->getOtEntry(draw.layer, obj->depth);
    draw.prim.any = GFX.funcs.getPrim();
    text->pos = obj->start;
    while (i < (s16)obj->visibleEnd - obj->start) {
        if (obj->finished != 0) {
            break;
        }
        c = FONT.decode(text->data + text->pos, (u8)text->sjis, obj->style);
        draw.code = c;
        draw.kind = (u32)(c << 16) >> 24;
        switch (processTextChar(obj, text, &draw, &text->pos)) {
        case 1:
            i++;
            goto drawGlyph;
        case 4:
            i++;
            continue;
        case 2:
        drawGlyph:
            if (draw.glyph->page == 0xFF) {
                draw.glyph = obj->style->glyphs;
            }
            x = obj->cursorX + (obj->x + draw.glyph->dx);
            y = obj->cursorY + (obj->y + draw.glyph->dy);
            clut = getClut(obj->texX + draw.glyph->clutX, obj->palette + (obj->texY + draw.glyph->clutY));
            u = draw.glyph->u;
            v = draw.glyph->v;
            if (obj->blend == -1) {
                tpage = getTPage(0, 1, obj->texX + (draw.glyph->page << 6), obj->texY);
            } else {
                tpage = getTPage(0, obj->blend & 3, obj->texX + (draw.glyph->page << 6), obj->texY);
            }
            if (!rotated) {
                if (i == 0) {
                    prevTpage = tpage;
                }
                if (tpage != prevTpage) {
                    SetDrawTPage(draw.prim.tpage, 0, 1, prevTpage);
                    addPrim(draw.ot, draw.prim.any);
                    prevTpage = tpage;
                    draw.prim.tpage++;
                }
                setlen(draw.prim.sprt, 4);
                setcode(draw.prim.sprt, 0x64);
                if (obj->blend != -1) {
                    setSemiTrans(draw.prim.sprt, 1);
                }
                draw.prim.sprt->r0 = draw.prim.sprt->g0 = draw.prim.sprt->b0 = 0x80;
                draw.prim.sprt->x0 = x;
                draw.prim.sprt->y0 = y;
                draw.prim.sprt->u0 = u;
                draw.prim.sprt->v0 = v;
                draw.prim.sprt->w = draw.glyph->w;
                draw.prim.sprt->h = draw.glyph->h;
                draw.prim.sprt->clut = clut;
                addPrim(draw.ot, draw.prim.any);
                draw.prim.sprt++;
                SetDrawTPage(draw.prim.tpage, 0, 1, tpage);
                addPrim(draw.ot, draw.prim.any);
                draw.prim.tpage++;
            } else {
                setlen(draw.prim.ft4, 9);
                setcode(draw.prim.ft4, 0x2C);
                draw.prim.ft4->r0 = draw.prim.ft4->g0 = draw.prim.ft4->b0 = 0x80;
                in[0].vx = in[2].vx = x - obj->pivotX;
                in[1].vx = in[3].vx = in[0].vx + draw.glyph->w;
                in[0].vy = in[1].vy = y - obj->pivotY;
                in[2].vy = in[3].vy = in[0].vy + draw.glyph->h;
                in[0].vz = in[1].vz = in[2].vz = in[3].vz = 0;
                for (j = 0; j < 4; j++) {
                    ApplyMatrixSV(&obj->mat, &in[j], &out);
                    (&draw.prim.ft4->x0)[j * 4] = out.vx + obj->pivotX;
                    (&draw.prim.ft4->y0)[j * 4] = out.vy + obj->pivotY;
                }
                draw.prim.ft4->u0 = draw.prim.ft4->u2 = u;
                draw.prim.ft4->u1 = draw.prim.ft4->u3 = draw.prim.ft4->u0 + draw.glyph->w - 1;
                draw.prim.ft4->v0 = draw.prim.ft4->v1 = v;
                draw.prim.ft4->v2 = draw.prim.ft4->v3 = draw.prim.ft4->v0 + draw.glyph->h - 1;
                if (obj->blend != -1) {
                    setSemiTrans(draw.prim.ft4, 1);
                }
                draw.prim.ft4->tpage = tpage;
                draw.prim.ft4->clut = clut;
                addPrim(draw.ot, draw.prim.any);
                draw.prim.ft4++;
            }
            if (obj->fixedSpacing != 0) {
                obj->cursorX += obj->spacingX;
            } else {
                obj->cursorX += draw.glyph->advance + draw.glyph->dx;
            }
            break;
        case 3:
            break;
        case 0:
        default:
            goto end;
        }
    }
    if (!rotated) {
        SetDrawTPage(draw.prim.tpage, 0, 1, tpage);
        addPrim(draw.ot, draw.prim.any);
        draw.prim.tpage++;
    }
end:
    GFX.funcs.setPrim(draw.prim.any);
}

/* Text window method: makes the whole current page visible at once */
void textWindowShowPage(TextWindow *obj) {
    s32 extra;
    s32 pos;
    s32 lines;
    s32 going;
    u8 *p;
    u8 index;
    s32 c;

    if (obj->state == 1) {
        pos = obj->start;
        extra = 0;
        lines = 0;
        going = 1;
        do {
            switch ((s32)((u32)(FONT.decode(obj->text[0].data + pos, (u8)obj->text[0].sjis, obj->style) << 16) >> 24)) {
            case 0:
            default:
                if (obj->text[0].sjis != 0) {
                    pos += 2;
                } else {
                    pos += 1;
                }
                break;
            case 1:
                pos += 2;
                break;
            case 2:
                /* summed as integers: as pointer arithmetic GCC swaps the operands */
                p = (u8 *)(pos + (s32)obj->text[0].data);
                c = p[1];
                switch (c) {
                default:
                    pos += FONT.codeLengths[c];
                    break;
                case 1:
                    if (++lines < obj->lines) {
                        pos += FONT.codeLengths[1];
                    } else {
                        going = 0;
                    }
                    break;
                case 2:
                    if (p[2] < 5) {
                        going = 0;
                    }
                    pos += FONT.codeLengths[2];
                    break;
                case 5:
                    index = p[2];
                    if (index < 6) {
                        extra += obj->text[index].len;
                        pos += FONT.codeLengths[5];
                    } else {
                        going = 0;
                    }
                    break;
                case 3:
                    going = 0;
                    break;
                }
                break;
            case 4:
                going = 0;
                break;
            }
        } while (going != 0);
        obj->visibleEnd = pos + extra;
    }
}

/* Text window method: one of the three font styles (others pick style 1) */
void textWindowSetStyle(TextWindow *obj, s32 style) {
    TextStyle *entry;

    if (style < 1 || style > 3) {
        style = 1;
    }
    entry = &FONT.styles[style];
    obj->style = entry;
    obj->blend = entry->blend;
}

/*
 * Text window method: types the text out a character every `delay` frames (0 or less: all at once)
 */
void textWindowSetTypeDelay(TextWindow *obj, s32 delay) {
    if (delay <= 0) {
        obj->typeTimer = 0;
        obj->typeDelay = 0;
        obj->visibleEnd = obj->text[0].len;
        return;
    }
    obj->visibleEnd = 0;
    obj->typeTimer = 0;
    obj->typeDelay = delay;
    obj->start = 0;
}

/* Text window method: moves the window */
void textWindowSetPos(TextWindow *obj, s16 x, s16 y) {
    obj->x = x;
    obj->y = y;
}

/* Text window method: the CLUT row of the glyphs */
void textWindowSetPalette(TextWindow *obj, u8 palette) {
    obj->palette = palette;
}

/*
 * Text window method: the glyphs' blending. An old-style definition, as
 * TextWindow.setBlend's unprototyped callers suit: they pass an int, which
 * it takes as a byte. setSpacing, setRightAlign, setLines and setUnusedC4
 * are old-style for the same reason.
 */
void textWindowSetBlend(obj, blend)
TextWindow *obj;
u8 blend;
{
    obj->blend = blend;
}

/* Text window method: a fixed advance and line height (0, 0: the style's) */
void textWindowSetSpacing(obj, x, y)
TextWindow *obj;
s16 x;
s16 y;
{
    if (x != 0 || y != 0) {
        obj->fixedSpacing = 1;
        obj->spacingX = x;
        obj->spacingY = y;
    } else {
        obj->fixedSpacing = 0;
        obj->spacingX = 0;
        obj->spacingY = 0;
    }
}

/* Text window method: shows or hides the window (an empty text stays hidden) */
void textWindowSetVisible(TextWindow *obj, u8 visible) {
    if (obj->text[0].len == 0) {
        obj->visible = 0;
    } else {
        obj->visible = visible;
    }
}

/* Text window method: aligns the text to its right end, or back to the left */
void textWindowSetRightAlign(obj, type)
TextWindow *obj;
u8 type;
{
    TextTools cls;

    if (type != 0) {
        initTextTools(&cls);
        obj->alignWidth = cls.measure(&obj->text[0], obj->style, obj->spacingX);
    } else {
        obj->alignWidth = 0;
    }
}

/* Text window method: fills the work buffers that control code 8 asks for with the player's name */
void textWindowInsertPlayerName(TextWindow *obj) {
    TextBuffer *text = &obj->text[0];
    s32 pos;
    u8 c;
    u8 index;

    if (obj->text[0].data != NULL) {
        for (pos = 0; pos < obj->text[0].len;) {
            switch ((s32)((u32)(FONT.decode(text->data + pos, (u8)text->sjis, obj->style) << 16) >> 24)) {
            case 0:
            case 3:
            default:
                if (text->sjis != 0) {
                    pos += 2;
                } else {
                    pos += 1;
                }
                break;
            case 1:
                pos += 2;
                break;
            case 2:
                c = text->data[pos + 1];
                if (c == 4) {
                    break;
                }
                if (c == 8) {
                    index = text->data[pos + 2];
                    textWindowSetSubText(obj, GAME.name, index);
                    obj->text[index].sjis = 0;
                    pos += FONT.codeLengths[8];
                } else {
                    pos += FONT.codeLengths[c];
                }
                break;
            case 4:
                return;
            }
        }
    }
}

/* Text window method: the sound each typed character plays (0 for none) */
void textWindowSetTypeSound(TextWindow *obj, s32 sound) {
    obj->typeSound = sound;
}

/* Text window method: scales the glyphs */
void textWindowSetScale(TextWindow *obj, s32 x, s32 y) {
    obj->scale.vz = ONE;
    obj->scale.vx = x;
    obj->scale.vy = y;
    obj->scaled = 1;
}

/* Text window method: the point the glyphs scale around */
void textWindowSetPivot(TextWindow *obj, s32 x, s32 y) {
    obj->pivotX = x;
    obj->pivotY = y;
}

/* Text window method: the OT depth the glyphs are drawn at */
void textWindowSetDepth(TextWindow *obj, s32 depth) {
    obj->depth = depth;
}

/* Text window method: the lines of a page */
void textWindowSetLines(obj, lines)
TextWindow *obj;
u8 lines;
{
    obj->lines = lines;
}

/* Text window method: sets unusedC4, which nothing reads */
void textWindowSetUnusedC4(obj, value)
TextWindow *obj;
u8 value;
{
    obj->unusedC4 = value;
}

/* Text window method: whether the end of the text was reached */
s32 textWindowIsFinished(TextWindow *obj) {
    return obj->finished;
}

/* Text window method: whether the window is shown */
s32 textWindowIsVisible(TextWindow *obj) {
    return obj->visible;
}

/* Text window method: whether the text waits for a button (control code 2) */
s32 textWindowIsWaitingForButton(TextWindow *obj) {
    return obj->substate == 1;
}

/*
 * Picks the glyph of the next character, or runs its control code; returns what the drawing does
 * next
 */
s32 processTextChar(TextWindow *obj, TextBuffer *text, TextDraw *draw, s16 *pos) {
    TextStyle *style;
    s32 c;
    s32 ret;
    s32 n;

    switch (draw->kind) {
    case 2:
        if (text->sjis != 0) {
            if ((u8)draw->code == 1) {
                ret = TEXT_CODE_HANDLERS[1](obj, text, draw);
                *pos += 1;
            } else {
                return TEXT_CODE_HANDLERS[0](obj, text, draw);
            }
        } else {
            c = text->data[*pos + 1];
            if (TEXT_CODE_HANDLERS[c] == NULL) {
                return TEXT_CODE_HANDLERS[0](obj, text, draw);
            }
            ret = TEXT_CODE_HANDLERS[c](obj, text, draw);
            if (ret & 0x8000) {
                *pos += FONT.codeLengths[c];
            }
        }
        return ret & ~0x8000;
    case 0:
        draw->glyph = &obj->style->glyphs[draw->code - 4];
        if (text->sjis != 0) {
            *pos += 2;
        } else {
            *pos += 1;
        }
        break;
    case 1:
        style = obj->style;
        n = (u8)draw->code;
        if (n <= style->iconCount && n > 0) {
            draw->glyph = &style->icons[n - 1];
        } else {
            draw->glyph = obj->style->glyphs;
        }
        *pos += 2;
        break;
    case 4:
        obj->finished = 1;
        *pos += 1;
        return 3;
    default:
        draw->glyph = obj->style->glyphs;
        if (text->sjis != 0) {
            *pos += 2;
        } else {
            *pos += 1;
        }
        break;
    }
    return 1;
}

/* Control codes 0, 7 and 9: show the page, or (code 7) start the text after a speaker's name */
s32 textCodeDefault(TextWindow *obj, TextBuffer *buf) {
    switch (buf->data[buf->pos + 1]) {
    case 0:
    default:
        textWindowShowPage(obj);
        break;
    case 7:
        obj->start = buf->pos + 2;
        break;
    }
    return 0;
}

/* Control code 1: a new line, or a new page when the page is full */
s32 textCodeNewLine(TextWindow *obj, TextBuffer *buf, TextDraw *draw) {
    if (++draw->lineCount == 1) {
        draw->pageStart = buf->pos + 2;
    } else if (draw->lineCount >= obj->lines) {
        obj->start = draw->pageStart;
        if (obj->lines >= 2) {
            obj->lines--;
            textWindowShowPage(obj);
            obj->lines++;
        }
        return 0x8000;
    }
    obj->cursorX = 0;
    if (obj->fixedSpacing != 0) {
        obj->cursorY += obj->spacingY;
    } else {
        obj->cursorY += obj->style->lineHeight;
    }
    return 0x8004;
}

/* Control code 2: waits for one of TEXT_WAIT_BUTTONS */
s32 textCodeWaitButton(TextWindow *obj, TextBuffer *buf) {
    if (buf->data[buf->pos + 2] < 5) {
        obj->state = 2;
        obj->substate = 1;
        if (buf->data[buf->pos + 2] < 1 || buf->data[buf->pos + 2] > 4) {
            obj->step = 0;
        } else {
            obj->step = buf->data[buf->pos + 2];
        }
        obj->counter = buf->pos + 2;
        return 0;
    }
    return 0x8003;
}

/* Control code 3: starts a new page */
s32 textCodePageBreak(TextWindow *obj, TextBuffer *buf) {
    u8 c;

    if (obj->typeDelay != 0) {
        obj->visibleEnd = buf->pos + 2;
    } else {
        obj->visibleEnd = buf->len;
    }
    while (buf->pos < buf->len) {
        switch ((s32)((u32)(FONT.decode(buf->data + buf->pos, (u8)buf->sjis, obj->style) << 16) >> 24)) {
        case 0:
        case 1:
        default:
            obj->start = buf->pos;
            buf->pos = buf->len + 1;
            break;
        case 2:
            c = buf->data[buf->pos + 1];
            if (c == 5 || c == 8) {
                obj->start = buf->pos;
                buf->pos = buf->len + 1;
            } else {
                buf->pos += FONT.codeLengths[c];
            }
            break;
        case 4:
            obj->start = buf->pos - 1;
            return 3;
        }
    }
    return 0;
}

/* Control code 4: skipped */
s32 textCodeIgnore(void) {
    return 0x8003;
}

/* Control code 5: draws the next character of a work buffer in its place */
s32 textCodeInsert(TextWindow *obj, TextBuffer *buf, TextDraw *draw) {
    u8 index = buf->data[buf->pos + 2];
    s16 c;
    s32 ret;

    if (obj->text[index].data == NULL) {
        textWindowSetSubText(obj, STR_MESSAGE_NOT_SET, index);
        return 0x8003;
    }
    if (obj->text[index].pos >= obj->text[index].len) {
        return 0x8003;
    }
    c = FONT.decode(obj->text[index].data + obj->text[index].pos, (u8)obj->text[index].sjis, obj->style, obj->text[index].pos);
    draw->code = c;
    draw->kind = (u32)(c << 16) >> 24;
    if (draw->kind == 2) {
        return 0x8000;
    }
    if (obj->typeDelay != 0) {
        return processTextChar(obj, &obj->text[index], draw, &obj->text[index].pos);
    }
    ret = processTextChar(obj, &obj->text[index], draw, &obj->text[index].pos);
    if (ret == 1) {
        return 2;
    }
    return ret;
}

/* Control code 6: pauses the typing for some frames, once */
s32 textCodePause(TextWindow *obj, TextBuffer *buf) {
    if (buf->data[buf->pos + 2] < 0xFF) {
        obj->state = 2;
        obj->substate = 0;
        obj->step = buf->data[buf->pos + 2];
        buf->data[buf->pos + 2] = 0xFF;
        obj->typeTimer = 0;
        return 0x8000;
    }
    return 0x8003;
}

/* Control code 8: puts the player's name into a work buffer, once */
s32 textCodePlayerName(TextWindow *obj, TextBuffer *buf) {
    if ((u8)buf->data[buf->pos + 2] < 6) {
        textWindowSetSubString(obj, GAME.name, -1, (u8)buf->data[buf->pos + 2]);
        buf->data[buf->pos + 2] = 6;
    }
    return 0x8003;
}

/*
 * The text window task: types and draws the text, waits where the control codes say, frees the
 * buffers at the end
 */
void updateTextWindow(TextWindow *obj) {
    s32 i;

    switch (obj->state) {
    case 0:
    default:
        obj->nextState(obj);
        break;
    case 1:
        if (obj->visible != 0) {
            if (obj->typeDelay > 0 && obj->finished == 0) {
                if (++obj->typeTimer > obj->typeDelay) {
                    obj->typeTimer = 0;
                    obj->visibleEnd++;
                    if (obj->typeSound != 0) {
                        SOUND.playSound(obj->typeSound);
                    }
                }
            }
            textWindowDraw(obj);
        }
        break;
    case 2:
        switch (obj->substate) {
        case 0:
        default:
            textWindowDraw(obj);
            if (++obj->typeTimer > obj->step) {
                obj->typeTimer = 0;
                obj->setState(obj, TASK_RUN);
            }
            break;
        case 1:
            if ((PAD.getPressed(0) >> PAD.getButtonBit(0, TEXT_WAIT_BUTTONS[obj->step])) & 1) {
                obj->text[0].data[obj->counter] = 5;
                obj->typeTimer = obj->typeDelay;
                obj->setState(obj, TASK_RUN);
            }
            textWindowDraw(obj);
            break;
        }
        break;
    case 3:
        for (i = 0; i < 6; i++) {
            if (obj->text[i].data != NULL) {
                HEAP.free(obj->text[i].data);
            }
        }
        break;
    }
}

/* A text window on a layer, in a font style, at (x, y) */
TextWindow *createTextWindow(s16 layerId, s16 style, s16 x, s16 y) {
    TextWindow *ret;
    TextWindow *obj = createTask(updateTextWindow, sizeof(TextWindow), 0);

    obj->setText = textWindowSetText;
    obj->setString = textWindowSetString;
    obj->setNumber = textWindowSetNumber;
    obj->setSubText = textWindowSetSubText;
    obj->setSubString = textWindowSetSubString;
    obj->draw = textWindowDraw;
    obj->showPage = textWindowShowPage;
    obj->setStyle = textWindowSetStyle;
    obj->setTypeDelay = textWindowSetTypeDelay;
    obj->setPos = textWindowSetPos;
    obj->setPalette = textWindowSetPalette;
    obj->setBlend = textWindowSetBlend;
    obj->setSpacing = textWindowSetSpacing;
    obj->setVisible = textWindowSetVisible;
    obj->setRightAlign = textWindowSetRightAlign;
    obj->insertPlayerName = textWindowInsertPlayerName;
    obj->setTypeSound = textWindowSetTypeSound;
    obj->setScale = textWindowSetScale;
    obj->setPivot = textWindowSetPivot;
    obj->setDepth = textWindowSetDepth;
    obj->setLines = textWindowSetLines;
    obj->setUnusedC4 = textWindowSetUnusedC4;
    obj->isFinished = textWindowIsFinished;
    obj->isVisible = textWindowIsVisible;
    obj->isWaitingForButton = textWindowIsWaitingForButton;
    if (style < 1 || style > 3) {
        style = 1;
    }
    obj->layerId = layerId;
    ret = obj;
    ret->style = &FONT.styles[style];
    ret->x = x;
    ret->texX = 0x140;
    ret->y = y;
    ret->texY = 0;
    ret->lines = 1;
    ret->blend = ret->style->blend;
    ret->scale.vx = ret->scale.vy = ret->scale.vz = ONE;
    return ret;
}

/* The handlers of the control codes 0x02 <code> (processTextChar) */
s32 (*TEXT_CODE_HANDLERS[11])() = {
    textCodeDefault, textCodeNewLine, textCodeWaitButton, textCodePageBreak,
    textCodeIgnore, textCodeInsert, textCodePause, textCodeDefault,
    textCodePlayerName, textCodeDefault, NULL,
};

/* The buttons a wait for a button (code 2) can wait for */
s32 TEXT_WAIT_BUTTONS[5] = { PAD_CROSS, PAD_CIRCLE, PAD_CROSS, PAD_TRIANGLE, PAD_SQUARE };
