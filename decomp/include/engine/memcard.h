#ifndef DW3_MEMCARD_H
#define DW3_MEMCARD_H

/* Memory card saves (memcard/) */

#include "common.h"
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>

/* The bytes the card reads or writes at once: one sector */
#define CARD_SECTOR_SIZE 0x80

/* The blocks of the save file */
#define SAVE_BLOCKS 4

/* The icon frames a save may have */
#define SAVE_MAX_ICONS 3

/* CardHeader.type: this flag plus the icon frame count */
#define CARD_HEADER_ICONS 0x10

/* The files MEMCARD lists from a card's directory */
#define CARD_MAX_FILES 15

/*
 * MemCard.result, as libmcrd's MemCardSync reports it (McErr*), plus
 * CARD_ERR_NOT_STARTED when the command could not even be sent
 */
#define CARD_ERR_NONE 0
#define CARD_ERR_NO_CARD 1
#define CARD_ERR_INVALID 2 /* McErrCardInvalid: "Cannot access Memory Card" */
#define CARD_ERR_NEW_CARD 3
#define CARD_ERR_UNFORMATTED 4
#define CARD_ERR_NO_FILE 5 /* McErrFileNotExist: "File cannot be found" */
#define CARD_ERR_FILE_EXISTS 6 /* McErrAlreadyExist: "File already exists" */
#define CARD_ERR_FULL 7 /* McErrBlockFull: "There are not enough blocks" */
#define CARD_ERR_NOT_STARTED 8

/* MemCard.state: idle, or waiting for the command each operation sent */
enum MemCardState {
    MEMCARD_IDLE,
    MEMCARD_CHECKING,
    MEMCARD_ACCEPTING,
    MEMCARD_READING,
    MEMCARD_WRITING,
    MEMCARD_COMMAND,
};

/* readSave and writeSave's sections: the header, the info, then the data copies */
enum SaveSection { SAVE_SECTION_HEADER, SAVE_SECTION_INFO, SAVE_SECTION_DATA };

/* memCardCommand's operations */
enum MemCardOp { MEMCARD_OP_LIST, MEMCARD_OP_CREATE, MEMCARD_OP_FORMAT, MEMCARD_OP_UNFORMAT };

/* Memory card save header (first CARD_SECTOR_SIZE bytes of a save) */
typedef struct CardClut {
    /* 0x00 */ u8 data[0x20];
} CardClut;

typedef struct CardHeader {
    /* 0x00 */ char magic[2];
    /* 0x02 */ u8 type;
    /* 0x03 */ u8 blocks;
    /* 0x04 */ char title[64];
    /* 0x44 */ u8 reserved[28];
    /* 0x60 */ CardClut clut;
} CardHeader;

/* Same layout as the kernel's struct DIRENTRY */
typedef struct CardDirEntry {
    /* 0x00 */ char name[20];
    /* 0x14 */ s32 attr;
    /* 0x18 */ s32 size;
    /* 0x1C */ struct CardDirEntry *next;
    /* 0x20 */ s32 head;
    /* 0x24 */ char system[4];
} CardDirEntry;

/*
 * Memory card access (MEMCARD), one step per call: every operation returns
 * 0 while it runs, 1 when it succeeds and result + 1 when it fails.
 * The save file (SAVE_BLOCKS blocks) holds the header and 1-3 icon frames (a
 * sector each), an info section (infoSize) and data sections (dataSize each).
 */
typedef struct MemCard {
    /* 0x00 */ s32 state; /* MEMCARD_* */
    /* 0x04 */ u8 pad4[8]; /* nothing reads or writes them */
    /* 0x0C */ const char *fileName;
    /* 0x10 */ CardHeader header;
    /* 0x90 */ s32 cmd;
    /* 0x94 */ u32 result;
    /* 0x98 */ s32 retries;
    /* 0x9C */ s32 maxRetries;
    /* 0xA0 */ s32 restart; /* the command failed and must be reissued */
    /* 0xA4 */ long fileCount; /* long: MemCardGetDirentry writes it */
    /* 0xA8 */ CardDirEntry files[CARD_MAX_FILES];
    /* 0x300 */ s32 progress;
    /* 0x304 */ s32 offset;
    /* 0x308 */ s32 pad308; /* nothing reads or writes it */
    /* 0x30C */ s32 dataSize;
    /* 0x310 */ s32 infoSize;
    /* 0x314 */ s32 iconCount;
    /* 0x318 */ void *icons[SAVE_MAX_ICONS]; /* each frame's image (setSaveHeader) */
    /* 0x324 */ s32 iconIndex; /* the icon a save is writing (STGMCARD_runSaves) */
} MemCard;

/* The memory card functions (MEMCARD_FUNCS); like MemCard's operations, the
   card ones return 0 while they run */
typedef struct MemCardFuncs {
    /* 0x00 */ void (*init)(void);
    /* 0x04 */ void (*setFileName)(void);
    /* 0x08 */ void (*setHeader)(char *title, CardClut *clut, s32 count, void **icons);
    /* 0x0C */ s32 (*check)(s32 port);
    /* 0x10 */ s32 (*accept)(s32 port);
    /* 0x14 */ s32 (*read)(s32 port, void *buf, s32 size, s32 section);
    /* 0x18 */ s32 (*write)(s32 port, void *buf, s32 size, s32 section);
    /* 0x1C */ s32 (*list)(s32 port);
    /* 0x20 */ s32 (*create)(s32 port);
    /* 0x24 */ s32 (*format)(s32 port);
    /* 0x28 */ s32 (*unformat)(void);
    /* 0x2C */ s32 (*verifyChecksum)(void *data, s32 size, char expected);
    /* 0x30 */ u8 (*computeChecksum)(void *data, s32 size);
} MemCardFuncs;

void initMemCard(void);
void setSaveFileName(void);
void setSaveHeader(char *title, CardClut *clut, s32 count, void **icons);
s32 memCardCommand(s32 port, s32 op);
s32 listSaves(s32 port);
s32 createSave(s32 port);
s32 formatMemCard(s32 port);
s32 unformatMemCard(void);
s32 verifyChecksum(void *data, s32 size, char expected);
u8 computeChecksum(void *data, s32 size);
s32 checkMemCard(s32 port);
s32 acceptMemCard(s32 port);
s32 readSave(s32 port, void *buf, s32 size, s32 section);
s32 writeSave(s32 port, void *buf, s32 size, s32 section);
s32 syncMemCard(void);

/* The memory card library, declared here rather than from libmcrd.h, which
   takes the buffers as u_long * and MemCardSync's result as a long *, where
   the game passes u8 buffers and compares the result unsigned */
void MemCardInit(long val);
void MemCardStart(void);
long MemCardSync(long mode, long *cmds, u_long *result);
long MemCardExist(long chan);
long MemCardAccept(long chan);
long MemCardCreateFile(long chan, const char *file, long blocks);
long MemCardFormat(long chan);
long MemCardReadFile(long chan, const char *file, void *adrs, long ofs, long bytes);
long MemCardWriteFile(long chan, const char *file, void *adrs, long ofs, long bytes);
long MemCardUnformat(long chan);
long MemCardGetDirentry(long chan, char *name, CardDirEntry *dir, long *files, long ofs, long max);

extern MemCard MEMCARD;
extern MemCardFuncs MEMCARD_FUNCS;
extern s32 MEMCARD_SYNC_CMDS[]; /* the MemCardSync command of each MEMCARD_OP_* */
extern char STR_ALL_FILES[]; /* the directory pattern that matches every file */

#endif /* DW3_MEMCARD_H */
