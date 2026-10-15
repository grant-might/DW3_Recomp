#ifndef DW3_TEXT_H
#define DW3_TEXT_H

/* Text windows, the cursor, the message and talk boxes, the font and the
   text helpers (text/) */

#include "common.h"
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include "engine/task.h"
#include "engine/gfx.h"

/*
 * A menu cursor: a text window showing one of the CURSOR_FRAMES glyphs.
 * It waits idleDelay vsyncs, then plays the five frames (substate 1) at
 * frameDelay vsyncs each.
 */
typedef struct Cursor {
    TASK_HEADER(Cursor);
    /* 0x50 */ s32 layerId;
    /* 0x54 */ s32 depth;
    /* 0x58 */ s32 x;
    /* 0x5C */ s32 y;
    /* 0x60 */ s32 palette; /* PALETTE_*, its text window's */
    /* 0x64 */ s32 visible;
    /* 0x68 */ s32 dirty;
    /* 0x6C */ s32 frame;
    /* 0x70 */ s32 time;
    /* 0x74 */ s32 idleDelay;
    /* 0x78 */ s32 frameDelay;
    /* 0x7C */ s32 still; /* stay on frame 0 */
    /* 0x80 */ void (*setVisible)(); /* (cursor, visible) */
    /* 0x84 */ void (*setPos)(); /* (cursor, x, y) */
    /* 0x88 */ void (*setPalette)();
    /* 0x8C */ void (*setIdleDelay)();
    /* 0x90 */ void (*setFrameDelay)();
    /* 0x94 */ void (*setStill)();
} Cursor;

/*
 * The frame of a message box: two halves of the bottom panel that grow from
 * the sides (substate 0), stay with a "next" arrow when showArrow is set
 * (substate 1), and fade out through five palette rows (substate 2).
 */
typedef struct MessageBoxFrame {
    TASK_HEADER(MessageBoxFrame);
    /* 0x50 */ s32 layerId;
    /* 0x54 */ s32 time;
    /* 0x58 */ s32 fadeRow;
    /* 0x5C */ s32 showArrow;
    /* 0x60 */ s32 arrowTime;
    /* 0x64 */ s32 arrowFrame;
    /* 0x68 */ s16 scale;
    /* 0x6A */ s16 scaleStep;
} MessageBoxFrame;

/* The children of a TalkBox */
typedef struct TalkBoxChildren {
    /* 0x00 */ struct ZoomBox *zoomBoxes[3];
    /* 0x0C */ struct TextWindow *windows[2]; /* speaker, message */
    /* 0x14 */ struct TalkBoxFrame *frame;
} TalkBoxChildren;

/*
 * A box with a speaker's name and a message (createTalkBox). It opens and
 * closes with three ZoomBox outlines (substate 0 opens, 1 closes) and
 * type (0-3) picks one of the TALK_BOX_LAYOUTS.
 */
typedef struct TalkBox {
    TASK_HEADER(TalkBox);
    /* 0x50 */ void *strings;
    /* 0x54 */ s32 layerId;
    /* 0x58 */ s32 type;
    /* 0x5C */ s32 timer;
    /* 0x60 */ s16 x;
    /* 0x62 */ s16 y;
    /* 0x64 */ s16 w;
    /* 0x66 */ s16 h;
    /* 0x68 */ void (*setPos)(struct TalkBox *box, s32 x, s32 y);
} TalkBox;

/* Draws a TalkBox's panel */
typedef struct TalkBoxFrame {
    TASK_HEADER(TalkBoxFrame);
    /* 0x50 */ TalkBox *parent;
    /* 0x54 */ s32 arrowFrame;
    /* 0x58 */ s32 arrowTime;
    /* 0x5C */ u8 showArrow;
} TalkBoxFrame;

/* Text helpers (initTextTools) */
typedef struct TextTools {
    /* 0x0 */ char *(*getString)(); /* (table, index) */
    /* 0x4 */ s32 (*measure)(); /* (TextBuffer *, style, spacing) */
    /* 0x8 */ void (*convert)(); /* (dst, src, mode): font codes <-> Shift-JIS */
} TextTools;

typedef struct TextBuffer {
    /* 0x0 */ u8 *data;
    /* 0x4 */ s16 cap;
    /* 0x6 */ s16 len;
    /* 0x8 */ s16 pos;
    /* 0xA */ s16 sjis; /* two bytes per character; else font codes */
} TextBuffer;

/*
 * The text windows' palettes (TextWindow.palette, setPalette): rows of the
 * font's CLUTs, named by the colour of the letters (white letters with a
 * grey shade in PALETTE_WHITE, the default)
 */
#define PALETTE_WHITE 0
#define PALETTE_BLUE 1 /* a light blue: what is highlighted or current */
#define PALETTE_DARK_BLUE 2
#define PALETTE_YELLOW 3
#define PALETTE_GREEN 4
#define PALETTE_RED 5
#define PALETTE_PURPLE 6
#define PALETTE_GREY 7 /* what is unavailable */

/*
 * A text window (createTextWindow): up to six text buffers (0 is shown, 1-5
 * are "work" buffers that control code 5 inserts), drawn glyph by glyph as
 * sprites, optionally typed out one character every typeDelay frames.
 * Control codes are 0x02 <code> <args> (TEXT_CODE_HANDLERS). The game's
 * own debug strings call these "messages" (STR_NULL_MESSAGE...).
 * States: 1 shows the text, 2 waits (substate 0: `step` frames,
 * substate 1: button TEXT_WAIT_BUTTONS[step], then patches the code at
 * `counter` so that it does not wait again), 3 frees the buffers.
 */
typedef struct TextWindow {
    TASK_HEADER(TextWindow);
    /* 0x50 */ struct TextStyle *style;
    /* 0x54 */ s32 layerId;
    /* 0x58 */ s32 depth;
    /* 0x5C */ TextBuffer text[6];
    /* 0xA4 */ u16 visibleEnd;
    /* 0xA6 */ s16 start;
    /* 0xA8 */ s16 typeTimer;
    /* 0xAA */ s16 typeDelay;
    /* 0xAC */ s16 texX;
    /* 0xAE */ s16 texY;
    /* 0xB0 */ s16 x;
    /* 0xB2 */ s16 y;
    /* 0xB4 */ s16 spacingX;
    /* 0xB6 */ s16 spacingY;
    /* 0xB8 */ s16 cursorX;
    /* 0xBA */ u16 cursorY;
    /* 0xBC */ s16 alignWidth;
    /* 0xBE */ s8 blend; /* -1 (TextStyle.blend 0xFF): opaque */
    /* 0xBF */ u8 lines;
    /* 0xC0 */ u8 palette; /* PALETTE_* */
    /* 0xC1 */ u8 visible;
    /* 0xC2 */ u8 fixedSpacing;
    /* 0xC3 */ u8 finished;
    /* 0xC4 */ u8 unusedC4; /* only setUnusedC4 writes it, and nothing calls that */
    /* 0xC8 */ s32 typeSound;
    /* 0xCC */ s32 scaled;
    /* 0xD0 */ VECTOR scale; /* ScaleMatrix's */
    /* 0xE0 */ s32 pivotX;
    /* 0xE4 */ s32 pivotY;
    /* 0xE8 */ SVECTOR rot;
    /* 0xF0 */ MATRIX mat;
    /* 0x110 */ void (*setText)();
    /* 0x114 */ void (*setString)();
    /* 0x118 */ void (*setNumber)();
    /* 0x11C */ void (*setSubText)();
    /* 0x120 */ void (*setSubString)();
    /* 0x124 */ void (*draw)();
    /* 0x128 */ void (*showPage)();
    /* 0x12C */ void (*setStyle)();
    /* 0x130 */ void (*setTypeDelay)();
    /* 0x134 */ void (*setPos)(struct TextWindow *obj, s16 x, s16 y);
    /* 0x138 */ void (*setPalette)(struct TextWindow *obj, u8 arg);
    /* 0x13C */ void (*setBlend)(); /* no prototype: callers would truncate u8 args */
    /* 0x140 */ void (*setSpacing)(); /* no prototype: callers would truncate s16 args */
    /* 0x144 */ void (*setVisible)(struct TextWindow *obj, u8 arg);
    /* 0x148 */ void (*setRightAlign)(); /* no prototype: callers would truncate u8 args */
    /* 0x14C */ void (*insertPlayerName)();
    /* 0x150 */ void (*setTypeSound)();
    /* 0x154 */ void (*setScale)();
    /* 0x158 */ void (*setPivot)();
    /* 0x15C */ void (*setDepth)();
    /* 0x160 */ void (*setLines)(); /* no prototype: callers would truncate u8 args */
    /* 0x164 */ void (*setUnusedC4)(); /* no prototype: callers would truncate u8 args */
    /* 0x168 */ s32 (*isFinished)(); /* s32: callers don't truncate the result to a byte */
    /* 0x16C */ s32 (*isVisible)(); /* s32: callers don't truncate the result to a byte */
    /* 0x170 */ s32 (*isWaitingForButton)();
} TextWindow;

/* Where the parts of a TalkBox go, for each of its four types */
typedef struct TalkBoxLayout {
    /* 0x00 */ s32 sprite;
    /* 0x04 */ DVECTOR spritePos; /* the four positions drawTalkBoxFrame walks */
    /* 0x08 */ DVECTOR sprite0Pos; /* also the ZoomBox's offset */
    /* 0x0C */ DVECTOR panel;
    /* 0x10 */ DVECTOR sprite2Pos; /* its y only for types 2 and 3 */
    /* 0x14 */ s16 nameX;
    /* 0x16 */ s16 nameY;
    /* 0x18 */ s16 textX;
    /* 0x1A */ s16 textY;
    /* 0x1C */ DVECTOR arrow; /* where the talk box arrow is drawn */
} TalkBoxLayout;

/*
 * The font (FONT): three text styles, the length of each control code, and
 * decode(), which turns the next character into kind << 8 | value:
 * kind 0 a glyph (value - 4 indexes glyphs), 1 an icon, 2 a control code,
 * 3 a missing character, 4 the end.
 */
typedef struct Font {
    /* 0x0 */ struct TextStyle *styles; /* [4], style 0 unused */
    /* 0x4 */ s32 *codeLengths;
    /* 0x8 */ void (*load)();
    /* 0xC */ s16 (*decode)(); /* (text, sjis, style) */
} Font;

/* Maps a Shift-JIS character to a glyph */
typedef struct GlyphMap {
    /* 0x0 */ u16 code;
    /* 0x2 */ u8 index;
    /* 0x3 */ u8 pad;
} GlyphMap;

typedef struct TextStyle {
    /* 0x00 */ u8 blend; /* 0xFF: opaque */
    /* 0x01 */ s8 lineHeight;
    /* 0x04 */ struct Glyph *glyphs;
    /* 0x08 */ struct Glyph *icons;
    /* 0x0C */ GlyphMap *sjisMap;
    /* 0x10 */ GlyphMap *iconMap;
    /* 0x14 */ s16 glyphCount;
    /* 0x16 */ s16 iconCount;
} TextStyle;

/* One glyph of a font sheet */
typedef struct Glyph {
    /* 0x0 */ u8 page; /* 0xFF: missing glyph */
    /* 0x1 */ u8 u;
    /* 0x2 */ u8 v;
    /* 0x3 */ u8 clutX;
    /* 0x4 */ u8 clutY;
    /* 0x5 */ u8 w;
    /* 0x6 */ u8 h;
    /* 0x7 */ s8 dx;
    /* 0x8 */ s8 dy;
    /* 0x9 */ u8 advance;
    /* 0xA */ u8 unusedA; /* h, or 10 for the 9-high glyphs; nothing reads it */
} Glyph;

/* Drawing state shared with the control-code handlers */
typedef struct TextDraw {
    /* 0x00 */ PrimPtr prim;
    /* 0x04 */ Layer *layer;
    /* 0x08 */ u_long *ot;
    /* 0x0C */ Glyph *glyph;
    /* 0x10 */ s32 pageStart;
    /* 0x14 */ s16 code; /* from Font.decode */
    /* 0x16 */ s16 kind;
    /* 0x18 */ s16 lineCount;
} TextDraw;

/* The children of a message box (createMessageBox) */
typedef struct MessageBox {
    /* 0x0 */ struct TextWindow *window;
    /* 0x4 */ struct MessageBoxFrame *frame;
} MessageBox;

/* A blue rectangle outline that zooms in (or out, when closing) around a TalkBox */
typedef struct ZoomBox {
    TASK_HEADER(ZoomBox);
    /* 0x50 */ s16 left;
    /* 0x52 */ s16 top;
    /* 0x54 */ s16 x; /* the zoom's centre */
    /* 0x56 */ s16 y;
    /* 0x58 */ s16 w;
    /* 0x5A */ s16 h;
    /* 0x5C */ s16 speed;
    /* 0x60 */ s32 zoom; /* 0-ONE */
    /* 0x64 */ s32 layerId;
    /* 0x68 */ s32 unused68; /* only cleared when the box starts; nothing reads it */
    /* 0x6C */ s16 offsetX;
    /* 0x6E */ s16 offsetY;
    /* 0x70 */ VECTOR scale;
    /* 0x80 */ SVECTOR rot;
    /* 0x88 */ VECTOR trans; /* the centre, set but not read */
    /* 0x98 */ MATRIX matrix;
    /* 0xB8 */ s32 closing;
    /* 0xBC */ s32 opened;
} ZoomBox;

void drawMessageBoxFrame(struct MessageBoxFrame *task);
void drawMessageBoxArrow(struct MessageBoxFrame *task);
void setTextBuffer(struct TextWindow *obj, struct TextBuffer *buf, const char *text);
void textWindowSetSubString(struct TextWindow *obj, char *text, s32 id, s32 index);
void drawZoomBox(ZoomBox *task);
void drawTalkBoxArrow(struct TalkBoxFrame *task);
void drawTalkBoxFrame(struct TalkBoxFrame *task);
void textWindowSetText(TextWindow *obj, const char *text);
void textWindowSetString(TextWindow *obj, char *text, s32 id);
void formatNumber(u8 *buf, s32 value);
void textWindowSetNumber(TextWindow *obj, u32 index, s32 value);
void textWindowSetSubText(TextWindow *obj, const char *text, s32 index);
void textWindowSetTypeDelay(TextWindow *, s32);
void textWindowSetPos(TextWindow *obj, s16 x, s16 y);
void textWindowSetPalette(TextWindow *obj, u8 palette);
void textWindowSetBlend(TextWindow *obj, s32 blend); /* old-style: u8 blend */
void textWindowSetSpacing(TextWindow *obj, s32 x, s32 y); /* old-style: s16 x, y */
void textWindowSetVisible(TextWindow *obj, u8 visible);
void textWindowSetRightAlign(TextWindow *obj, s32 type); /* old-style: u8 type */
void textWindowInsertPlayerName(TextWindow *obj);
void textWindowSetTypeSound(TextWindow *obj, s32 sound);
void textWindowSetScale(TextWindow *obj, s32 x, s32 y);
void textWindowSetPivot(TextWindow *obj, s32 x, s32 y);
void textWindowSetDepth(TextWindow *obj, s32 depth);
void textWindowSetLines(TextWindow *obj, s32 lines); /* old-style: u8 lines */
void textWindowSetUnusedC4(TextWindow *obj, s32 value); /* old-style: u8 value */
s32 textWindowIsFinished(TextWindow *obj);
s32 textWindowIsVisible(TextWindow *obj);
s32 textWindowIsWaitingForButton(TextWindow *obj);
void textWindowShowPage(TextWindow *obj);
void textWindowSetStyle(TextWindow *obj, s32 style);
void textWindowDraw(struct TextWindow *obj);
void cursorSetVisible(Cursor *task, s32 visible);
void cursorSetPos(Cursor *task, s32 x, s32 y);
void cursorSetPalette(Cursor *task, s32 palette);
void cursorSetIdleDelay(Cursor *task, s32 delay);
void cursorSetFrameDelay(Cursor *task, s32 delay);
void cursorSetStill(Cursor *task, s32 still);
void updateCursor(Cursor *task, TextWindow **win);
Cursor *createCursor(s16 layerId, s32 depth, s16 x, s16 y);
void updateMessageBox(Task *task, struct MessageBox *data);
s32 processTextChar(TextWindow *obj, TextBuffer *text, TextDraw *draw, s16 *pos);
s32 textCodeDefault(TextWindow *obj, TextBuffer *buf);
s32 textCodeNewLine(TextWindow *obj, TextBuffer *buf, TextDraw *draw);
s32 textCodeWaitButton(TextWindow *obj, TextBuffer *buf);
s32 textCodePageBreak(TextWindow *obj, TextBuffer *buf);
s32 textCodeIgnore(void);
s32 textCodeInsert(TextWindow *obj, TextBuffer *buf, TextDraw *draw);
s32 textCodePause(TextWindow *obj, TextBuffer *buf);
s32 textCodePlayerName(TextWindow *obj, TextBuffer *buf);
void updateTextWindow(TextWindow *obj);
void updateZoomBox(struct ZoomBox *task);
ZoomBox *createZoomBox(s32 layerId, s16 x, s16 y, s32 w, s32 h, s32 type);
void talkBoxSetPos(TalkBox *task, s32 x, s32 y);
void updateTalkBox(TalkBox *task, TalkBoxChildren *children);
TextWindow *createTextWindow(s16 id, s16 type, s16 x, s16 y);
void updateMessageBoxFrame(struct MessageBoxFrame *task);
MessageBoxFrame *createMessageBoxFrame(s32 layerId);
void updateTalkBoxFrame(TalkBoxFrame *task);
TalkBoxFrame *createTalkBoxFrame(TalkBox *parent);
Task *createMessageBox(s32 layerId, void *strings, s32 index);
TalkBox *createTalkBox(s32 id, s16 x, s16 y, void *strings, s32 index, u32 type);
void bindTextTools(TextTools *obj);
char *getString(s32 *table, s32 index);
s32 measureText(TextBuffer *text, TextStyle *style, s32 spacing); /* old-style: s16 spacing */
void initTextTools(TextTools *obj);
void convertText(void *buf, void *text, s32 mode);
void loadFont(void);
s16 decodeChar(u8 *s, u8 mode, TextStyle *font);

extern const char STR_NULL_MESSAGE[];
extern const char STR_BAD_DIGIT_BUFFER[];
extern const char STR_BAD_EXT_BUFFER[];
extern const char STR_MESSAGE_NOT_SET[];
extern char *CURSOR_FRAMES[];
extern s32 TEXT_WAIT_BUTTONS[];
extern Font FONT;
extern s32 (*TEXT_CODE_HANDLERS[])(); /* (obj, text, wait): most handlers take only the first two */
extern GlyphMap FONT_GLYPH_MAP[];
extern GlyphMap FONT_ICON_MAP[];
extern Glyph FONT_GLYPHS_1[]; /* the three font sizes' glyphs and icons (text_tools.c) */
extern Glyph FONT_ICONS_1[];
extern Glyph FONT_GLYPHS_2[];
extern Glyph FONT_ICONS_2[];
extern Glyph FONT_GLYPHS_3[];
extern Glyph FONT_ICONS_3[];
extern TalkBoxLayout TALK_BOX_LAYOUTS[];

#endif /* DW3_TEXT_H */
