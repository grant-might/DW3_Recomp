#ifndef DW3_FILE_H
#define DW3_FILE_H

/* The disc file table, the CD reader, the file cache and the decompressor
   (file/) */

#include "common.h"
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>
#include "engine/task.h"

/* The disc's file table (FILE_TABLE): files are numbered, not named */
typedef struct FileTableFuncs {
    /* 0x0 */ s32 (*exists)(s32 file);
    /* 0x4 */ s32 (*getSectorCount)(s32 file);
    /* 0x8 */ s32 (*getSector)(s32 file);
    /* 0xC */ void (*getPos)(s32 file, s32 offset, void *loc);
} FileTableFuncs;

/* A file in the cache */
/* FileSlot.state */
enum FileSlotState { FILE_FREE, FILE_QUEUED, FILE_READING, FILE_LOADED };

/* The files the cache holds at most */
#define FILE_CACHE_SLOTS 64

typedef struct FileSlot {
    /* 0x0 */ s16 state; /* FILE_* */
    /* 0x2 */ s16 marked;
    /* 0x4 */ s32 file;
    /* 0x8 */ s32 lastUsed; /* GFX time */
    /* 0xC */ void *data;
} FileSlot;

/* What the CD reader waits for (CdReader.state): the command each state
   sent to complete, or, in CD_READ_SECTORS, the sectors and then CdlPause */
enum CdReadState {
    CD_READ_IDLE,
    CD_READ_SETLOC,
    CD_READ_SETMODE,
    CD_READ_READN,
    CD_READ_SECTORS,
};

/* The bytes of data in a sector (Mode 2 Form 1), as the reader reads them */
#define CD_SECTOR_SIZE 0x800

/* Reads whole sectors of a file with CdlReadN, from callbacks */
typedef struct CdReader {
    /* 0x00 */ s32 state; /* CD_READ_* */
    /* 0x04 */ s32 file;
    /* 0x08 */ s32 offset; /* in sectors */
    /* 0x0C */ s32 sectorCount;
    /* 0x10 */ u8 *buffer;
    /* 0x14 */ s32 *done; /* set to 1 when the read is complete */
    /* 0x18 */ u8 loc[4];
    /* 0x1C */ s32 sector;
    /* 0x20 */ s32 sectorsLeft;
    /* 0x24 */ u8 *dst;
    /* 0x28 */ s32 nextSector;
    /* 0x2C */ s32 (*isBusy)(void);
    /* 0x30 */ void (*read)(s32 file, s32 offset, s32 size, void *buf, s32 *done);
} CdReader;

/*
 * What cdCheckSector reads first of each sector, the 3 words that
 * CdlModeSize1 puts before its data: the sector's header, whose position it
 * compares with the one it expects, and the XA subheader, which nothing
 * reads. The buffer takes 16 bytes in the .bss (the sound buffers follow
 * 0x10 after it), so 4 more than it reads.
 */
typedef struct CdSectorHeader {
    /* 0x0 */ CdlLOC pos;
    /* 0x4 */ u_char subheader[8];
    /* 0xC */ u_char pad[4];
} CdSectorHeader;

/*
 * The file cache (FILE_CACHE): up to 64 files loaded in the heap (tag 3),
 * read in the background one at a time and evicted least recently used
 * first when the heap is full.
 */
typedef struct FileCache {
    /* 0x000 */ s32 pending;
    /* 0x004 */ FileSlot slots[FILE_CACHE_SLOTS];
    /* 0x404 */ s32 (*isLoading)(s32 file);
    /* 0x408 */ void (*evictOldest)(void);
    /* 0x40C */ void (*request)(s32 file);
    /* 0x410 */ void (*update)(void);
    /* 0x414 */ void *(*load)(u32 file);
    /* 0x418 */ void (*free)(s32 file);
    /* 0x41C */ void (*freeAll)(void);
    /* 0x420 */ void (*freeFrom)(u32 addr);
    /* 0x424 */ void *(*getEntry)(u32 fileAndIndex);
    /* 0x428 */ void *(*getArchiveEntry)(s32 index, void *archive);
    /* 0x42C */ void (*markCached)(void);
    /* 0x430 */ void (*touchMarked)(void);
} FileCache;

/* The first word of "RLEN" data, which its unpacked size follows */
#define RLEN_MAGIC 0x4E454C52

/*
 * Expands "RLEN" data (a run-length encoding: a byte n < 0x80 copies n
 * bytes, n | 0x80 repeats the next byte n times, 0 ends), all at once
 * (run) or chunkSize bytes a frame (start, then substate 1 until done).
 */
typedef struct Decompressor {
    TASK_HEADER(Decompressor);
    /* 0x50 */ s32 *data;
    /* 0x54 */ s32 *begin;
    /* 0x58 */ s32 compressed;
    /* 0x5C */ s32 size;
    /* 0x60 */ s32 bufferSize;
    /* 0x64 */ void *buffer;
    /* 0x68 */ void *src; /* the next packed byte */
    /* 0x6C */ void *dst;
    /* 0x70 */ s32 chunkSize;
    /* 0x74 */ void *(*run)();
    /* 0x78 */ void *(*getData)();
    /* 0x7C */ void (*start)();
    /* 0x80 */ void (*free)();
} Decompressor;

s32 isFileLoading(s32);
FileSlot *findFileSlot(s32 file);
FileSlot *findFreeFileSlot(void);
FileSlot *findOldestFile(void);
void evictOldestFile(void);
void requestFile(s32);
void updateFileCache(void);
void waitForFile(s32 file);
void *loadFile(u32 file);
void freeFile(s32 file);
void freeAllFiles(void);
void freeFilesFrom(u32 addr);
void *getFileEntry(u32 fileAndIndex);
void *getArchiveEntry(s32 index, void *archive);
void markCachedFiles(void);
void touchMarkedFiles(void);
void cdSyncCallback(s32 status, u_char *result);
s32 cdCheckSector(void);
void cdReadyCallback(s32 status, u_char *result);
s32 isCdReading(void);
void startCdRead(void);
void readFile(s32 file, s32 offset, s32 size, void *buffer, s32 *done);
s32 fileExists(s32 file);
s32 getFileSectorCount(s32 file);
s32 getFileSector(s32 file);
void getFilePos(s32 file, s32 offset, void *pos);

void *decompressorRun(Decompressor *task, s32 *data);
void decompressorFree(Decompressor *task);
void decompressorSetData(Decompressor *task, s32 *data);
void decompressorAllocBuffer(Decompressor *task);
void decompressorStep(Decompressor *task);
Decompressor *createDecompressor(void);
void decompressorStart(Decompressor *task, s32 *data, s32 chunkSize);
void *decompressorGetData(Decompressor *task);
void updateDecompressor(Decompressor *task);

extern FileTableFuncs FILE_TABLE;
extern CdReader CD_READER;
extern CdSectorHeader CD_SECTOR_HEADER;
extern FileCache FILE_CACHE;
extern s32 FILE_SECTORS[];
extern u16 FILE_SECTOR_COUNTS[];

/*
 * Files the engine loads by number. The European disc numbers its files
 * differently, and has each text file once per language, in a row:
 * TEXT_FILE() gives the copy of the language the player picked (LANGUAGE,
 * set by CNTY_SEL), copy 1 being at the USA version's number. Every disc
 * has all the copies; the Japanese version reads copy 0, Japanese, which
 * is the European version's LANGUAGE 0 (NAME_ENTRY_JAPANESE), so it is
 * the European code with LANGUAGE a constant. The Japanese disc otherwise
 * numbers its files as the USA one does: every file of the USA disc is at
 * the same number on it (its 55 more stages' files come after), so the
 * Japanese version takes the USA version's file numbers.
 */
#if VERSION_US
#define FILE_MENU_SPRITES 0x277 /* the menu graphics, a sprite sheet */
#define FILE_FONT 0x278
#define TEXT_FILE(file) (file)
#elif VERSION_EU
#define FILE_MENU_SPRITES 0x286
#define FILE_FONT 0x287
#define TEXT_FILE(file) (LANGUAGE + (file) - 1)
extern s32 LANGUAGE; /* 2-5 */
#elif VERSION_JP
#define FILE_MENU_SPRITES 0x277
#define FILE_FONT 0x278
#define TEXT_FILE(file) (LANGUAGE + (file) - 1)
#define LANGUAGE 0
#endif

/* The text files, by their USA disc names (US<name>.BIN), for TEXT_FILE() */
#define TEXT_AMATERASU_MAP 0x02 /* AMTMAP: the map screen's in the second half (STSTATUS_getGameHalf) */
#define TEXT_ASUKA_MAP 0x09 /* ASKMAP: the map screen's before */
#define TEXT_CARD_GAME 0x10 /* CARDGM: the card battle's */
#define TEXT_CARD_NAMES 0x17 /* CARDNM */
#define TEXT_CARD_EFFECTS 0x1E /* CARDST */
#define TEXT_CARD_ALBUM 0x25 /* CRDABM: STCRDABM's */
#define TEXT_DECK_EDITOR 0x2C /* CRDDEK */
#define TEXT_CARD_SHOP 0x33 /* CRDSHP: STCRDSHP's, and STCRDDEK's */
#define TEXT_DIGI_LAB 0x3A /* DGLABO: STGDGLAB's */
#define TEXT_DIGIMON_NAMING 0x41 /* DGNMET: STDGNAME's */
#define TEXT_DIGIMON_INFO 0x48 /* DIGINF */
#define TEXT_DIGIMON_NAMES 0x4F /* DIGNAM */
#define TEXT_FIGHT_REPORT 0x56 /* FGTRPT: STFGTREP's */
#define TEXT_INN_NAMES 0x5D /* HTLNAM */
#define TEXT_ITEM_INFO 0x64 /* ITMINF */
#define TEXT_ITEM_NAMES 0x6B /* ITMNAM */
#define TEXT_ITEM_SHOP 0x72 /* ITSHOP: STITSHOP's */
#define TEXT_MEMORY_CARD 0x79 /* MEMCRD */
#define TEXT_BATTLE_MENU 0x80 /* MFIGHT: WFIGHTMN's */
#define TEXT_NAME_ENTRY 0x87 /* NAMEDT: the keyboard's */
#define TEXT_ONLINE 0x8E /* NAMEET: the Digimon Online registration */
#define TEXT_SHOP_NAMES 0x95 /* SHPNAM */
#define TEXT_SKILL_INFO 0x9C /* SKLINF */
#define TEXT_SKILL_NAMES 0xA3 /* SKLNAM */
#define TEXT_AREA_NAMES 0xAA /* STAREA */
#define TEXT_STATUS 0xB1 /* STATUS: the field menu's and STSTATUS's */
#define TEXT_STAGE_NAMES 0xB8 /* STNAME */

/* The battle menu's files (WFIGHTMN, WFIGHTTS): FILE_BATTLE_IMAGES is an
   archive whose entries go through FILE_CACHE.getEntry, the four after it
   are archives of images for VRAM */
#if VERSION_US || VERSION_JP
#define FILE_BATTLE_MENU 0x445
#define FILE_BATTLE_IMAGES 0x446
#define FILE_BATTLE_IMAGES_1 0x78F
#define FILE_BATTLE_IMAGES_2 0x79F
#define FILE_BATTLE_IMAGES_3 0x7D2
#define FILE_BATTLE_IMAGES_4 0x7D4
#elif VERSION_EU
#define FILE_BATTLE_MENU 0x455
#define FILE_BATTLE_IMAGES 0x456
#define FILE_BATTLE_IMAGES_1 0x79E
#define FILE_BATTLE_IMAGES_2 0x7AE
#define FILE_BATTLE_IMAGES_3 0x7E1
#define FILE_BATTLE_IMAGES_4 0x7E3
#endif

#endif /* DW3_FILE_H */
