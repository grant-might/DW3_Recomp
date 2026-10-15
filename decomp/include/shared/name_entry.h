#ifndef NAME_ENTRY_H
#define NAME_ENTRY_H

/*
 * The on-screen keyboard that the overlays use to type a name (STCRDDEK's
 * deck names, for example). The name is kept as maxLength full-width
 * Shift-JIS characters, padded with spaces. STCRDDEK, STPLNMET and STDGNAME
 * each have a copy of its code, from src/shared/name_entry/; the
 * overlay's header defines, before including this one, OVL_NAME,
 * NAME_ENTRY_HAS_UNUSED98 and NAME_ENTRY_HAS_HIDE (see NameEntry), and
 * NAME_ENTRY_SPRITES and NAME_ENTRY_FILE_KEYBOARD, its sprite bank and its
 * archive of the keyboard's images.
 */

#include "engine/game.h"

#define SJIS_SPACE 0x4081 /* full-width space, 0x81 0x40 */

/* The keyboard's special keys: row * 15 + column of their first cell */
#define NAME_KEY_DELETE 0x46 /* row 4: clears the character (the one before on a space) */
#define NAME_KEY_SPACE 0x55 /* row 5 */
#define NAME_KEY_LEFT 0x64 /* row 6: moves the cursor */
#define NAME_KEY_RIGHT 0x65
#define NAME_KEY_END 0x67 /* row 6, column 13: where Start goes */

/*
 * The Japanese keyboard (three pages) is for language 0 of the European
 * version, the Japanese version's only one; the USA version only has the
 * one page.
 */
#if VERSION_US
#define NAME_ENTRY_JAPANESE 0
#elif VERSION_EU || VERSION_JP
#define NAME_ENTRY_JAPANESE (LANGUAGE == 0)
#endif

/* A sprite of the keyboard drawn per language, one after the other (USA: the first) */
#if VERSION_US
#define NAME_ENTRY_TEXT_SPRITE(sprite) ((sprite) + 1)
#elif VERSION_EU || VERSION_JP
#define NAME_ENTRY_TEXT_SPRITE(sprite) (LANGUAGE + (sprite))
#endif

/* A key of the keyboard bigger than one cell: its sprite and where it goes */
typedef struct BigKey {
    /* 0x0 */ s32 sprite;
    /* 0x4 */ s32 x;
    /* 0x8 */ s32 y;
} BigKey;

/* A keyboard cell: 1 starts a key, 0 is empty and negative cells belong to a wide key */
typedef struct NameKey {
    /* 0x0 */ s8 kind;
    /* 0x1 */ u8 code;
} NameKey;

/* A page of the keyboard: 7 rows of 15 cells */
typedef struct KeyPage {
    /* 0x0 */ NameKey cells[7][15];
} KeyPage;

/* The texts of a keyboard page's three tabs */
typedef struct KeyTabs {
    /* 0x0 */ s32 texts[3];
} KeyTabs;

/* The character pages of the keyboard (as STDGNAME's and STPLNMET's) */
typedef struct NameKeyboard {
    /* 0x0 */ s32 pageCount;
    /* 0x4 */ KeyTabs *tabTexts; /* one per page */
    /* 0x8 */ KeyPage *pages;
} NameKeyboard;

typedef struct NameEntryWindows {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ TextWindow *name;
    /* 0x08 */ TextWindow *tabs[3];
    /* 0x14 */ TextWindow *unused14[3]; /* nothing creates or reads them */
    /* 0x20 */ TextWindow *leftLabel;
    /* 0x24 */ TextWindow *rightLabel;
    /* 0x28 */ TextWindow *l1Label;
    /* 0x2C */ TextWindow *r1Label;
    /* 0x30 */ TextWindow *message;
} NameEntryWindows;

/*
 * A name entry: the keyboard, the name typed so far and the panels that
 * open and close. STDGNAME's has a word more at 0x98, which moves what
 * follows by 4, and STPLNMET's a hide method before close: the including
 * overlay says so with NAME_ENTRY_HAS_UNUSED98 and NAME_ENTRY_HAS_HIDE (0
 * or 1). The offsets are STCRDDEK's. With three layouts, each overlay's
 * copy has a tag of its own (STCRDDEK_NameEntry, ...).
 */
typedef struct OVL_NAME(NameEntry) NameEntry;
struct OVL_NAME(NameEntry) {
    TASK_HEADER(OVL_NAME(NameEntry));
    /* 0x50 */ s32 mode;
    /* 0x54 */ s32 layer;
    /* 0x58 */ s32 depth;
    /* 0x5C */ s32 vramX; /* where the keyboard's images go in VRAM */
    /* 0x60 */ s32 vramY;
    /* 0x64 */ s32 partner; /* -1: none */
    /* 0x68 */ s32 partnerFrame;
    /* 0x6C */ s32 partnerTime;
    /* 0x70 */ s32 clutRow;
    /* 0x74 */ s32 clutTime;
    /* 0x78 */ u16 name[12];
    /* 0x90 */ s32 cursor;
    /* 0x94 */ s32 maxLength;
#if NAME_ENTRY_HAS_UNUSED98
    /* 0x98 */ s32 unused98; /* nothing reads or writes it */
#endif
    /* 0x98 */ s32 column;
    /* 0x9C */ s32 row;
    /* 0xA0 */ s32 keyFrame;
    /* 0xA4 */ s32 keyTime;
    /* 0xA8 */ s32 active;
    /* 0xAC */ s32 page;
    /* 0xB0 */ s32 arrowFrame;
    /* 0xB4 */ s32 arrowTime;
    /* 0xB8 */ s32 unusedB8; /* nothing reads or writes it */
    /* 0xBC */ PanelAnim unusedBC; /* given a duration of 10, as the two below, and never run */
    /* 0xCC */ PanelAnim keyboardScale;
    /* 0xDC */ PanelAnim messageScale;
    /* 0xEC */ void (*getName)(NameEntry *task, char *out);
#if NAME_ENTRY_HAS_HIDE
    /* 0xF0 */ void (*hide)(NameEntry *task, s32 hide);
#endif
    /* 0xF0 */ void (*close)(NameEntry *task);
};

#endif /* NAME_ENTRY_H */
