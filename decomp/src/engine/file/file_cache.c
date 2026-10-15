#include "engine/game.h"
#include <libgs.h>
#include <libetc.h>
#include <libsnd.h>

/* The file cache */
FileCache FILE_CACHE = {
    0,
    {{0}},
    isFileLoading,
    evictOldestFile,
    requestFile,
    updateFileCache,
    loadFile,
    freeFile,
    freeAllFiles,
    freeFilesFrom,
    getFileEntry,
    getArchiveEntry,
    markCachedFiles,
    touchMarkedFiles,
};

/* The cache slot of a file, or NULL */
FileSlot *findFileSlot(s32 file) {
    FileSlot *slot;
    s32 i;

    for (slot = FILE_CACHE.slots, i = 0; i < FILE_CACHE_SLOTS; i++, slot++) {
        if (slot->file == file) {
            return slot;
        }
    }
    return NULL;
}

/* A free cache slot, or NULL */
FileSlot *findFreeFileSlot(void) {
    FileSlot *slot;
    s32 i;

    for (slot = FILE_CACHE.slots, i = 0; i < FILE_CACHE_SLOTS; i++, slot++) {
        if (slot->file == 0) {
            return slot;
        }
    }
    return NULL;
}

/* 0 once the file is in the cache; requests it if needed */
s32 isFileLoading(s32 file) {
    FileSlot *slot = findFileSlot(file);

    if (slot != NULL) {
        slot->lastUsed = GFX.funcs.getTime();
        if (slot->state == FILE_LOADED) {
            return 0;
        }
    } else {
        requestFile(file);
    }
    return 1;
}

/* The loaded file used least recently (the biggest one on a tie) */
FileSlot *findOldestFile(void) {
    FileSlot *best = NULL;
    s32 bestSize = 0;
    s32 i = 0;
    s32 time = GFX.funcs.getTime();
    FileSlot *slot;

    for (slot = FILE_CACHE.slots; i < FILE_CACHE_SLOTS; i++, slot++) {
        if (slot->file != 0 && slot->state == FILE_LOADED && time >= slot->lastUsed) {
            if (time != slot->lastUsed || FILE_TABLE.getSectorCount(slot->file) >= bestSize) {
                bestSize = FILE_TABLE.getSectorCount(slot->file);
                time = slot->lastUsed;
                best = slot;
            }
        }
    }
    return best;
}

/* Frees the loaded file used least recently, to make room in the heap */
void evictOldestFile(void) {
    FileSlot *slot = findOldestFile();

    HEAP.free(slot->data);
    slot->file = 0;
    slot->data = NULL;
    slot->lastUsed = 0;
    slot->marked = 0;
    slot->state = FILE_FREE;
}

/* Queues a file for the cache (just marks it used when it is there) */
void requestFile(s32 file) {
    FileSlot *slot = findFileSlot(file);

    if (slot != NULL) {
        slot->lastUsed = GFX.funcs.getTime();
        return;
    }
    slot = findFreeFileSlot();
    slot->file = file;
    slot->data = HEAP.alloc(FILE_TABLE.getSectorCount(file) * CD_SECTOR_SIZE, MEM_FILE_CACHE);
    slot->state = FILE_QUEUED;
    slot->lastUsed = 0;
    slot->marked = 0;
    FILE_CACHE.pending = 1;
}

/* Starts reading the next queued file and marks the one read as loaded */
void updateFileCache(void) {
    FileSlot *slot;
    s32 i;
    s32 busy;
    s32 reading;

    if (FILE_CACHE.pending != 0 && CD_READER.isBusy() != 1) {
        slot = FILE_CACHE.slots;
        reading = 0;
        busy = 0;
        for (i = 0; i < FILE_CACHE_SLOTS; i++, slot++) {
            if (slot->file != 0) {
                switch (slot->state) {
                case FILE_READING:
                    slot->state = FILE_LOADED;
                    busy = 1;
                    slot->lastUsed = GFX.funcs.getTime();
                    break;
                case FILE_QUEUED:
                    busy = 1;
                    if (!reading) {
                        CD_READER.read(slot->file, 0, 0, slot->data, NULL);
                        slot->state = FILE_READING;
                        reading = busy;
                        slot->lastUsed = GFX.funcs.getTime();
                    }
                    break;
                }
            }
        }
        if (!busy) {
            FILE_CACHE.pending = 0;
        }
    }
}

/* Reads a file into the cache before returning */
void waitForFile(s32 file) {
    requestFile(file);
    do {
        updateFileCache();
    } while (isFileLoading(file) != 0);
}

/* Returns the file's data, reading it now if it is not in the cache */
void *loadFile(u32 file) {
    FileSlot *slot = findFileSlot(file);

    if (slot != NULL && slot->state == FILE_LOADED) {
        slot->lastUsed = GFX.funcs.getTime();
        return slot->data;
    }
    while (CD_READER.isBusy() != 0) {
    }
    waitForFile(file);
    return findFileSlot(file)->data;
}

/* Drops a loaded file from the cache */
void freeFile(s32 file) {
    FileSlot *slot = findFileSlot(file);

    if (slot != NULL && slot->state == FILE_LOADED) {
        HEAP.free(slot->data);
        slot->file = 0;
        slot->data = NULL;
        slot->lastUsed = 0;
        slot->marked = 0;
        slot->state = FILE_FREE;
    }
}

/* Drops every file from the cache */
void freeAllFiles(void) {
    FileSlot *slot;
    s32 i;

    for (slot = FILE_CACHE.slots, i = 0; i < FILE_CACHE_SLOTS; i++, slot++) {
        if (slot->file != 0) {
            HEAP.free(slot->data);
            slot->file = 0;
            slot->data = NULL;
            slot->lastUsed = 0;
            slot->marked = 0;
            slot->state = FILE_FREE;
        }
    }
}

/* Frees the files that reach past addr (to load something there) */
void freeFilesFrom(u32 addr) {
    FileSlot *slot = FILE_CACHE.slots;
    s32 i;
    u32 end;

    for (i = 0; i < FILE_CACHE_SLOTS; i++, slot++) {
        if (slot->file != 0) {
            end = (u32)slot->data + 0x20;
            end += FILE_TABLE.getSectorCount(slot->file) * CD_SECTOR_SIZE;
            if (end >= addr) {
                HEAP.free(slot->data);
                slot->file = 0;
                slot->data = NULL;
                slot->lastUsed = 0;
                slot->marked = 0;
                slot->state = FILE_FREE;
            }
        }
    }
}

/*
 * Files are often archives: a table of offsets from the start of the file.
 * getFileEntry((file << 16) | index) loads the file and returns that entry.
 */
void *getFileEntry(u32 fileAndIndex) {
    s32 index = fileAndIndex & 0xFFFF;
    s32 *table = loadFile(fileAndIndex >> 16);

    /* summed as integers: as pointer arithmetic GCC swaps the operands */
    return (void *)(table[index] + (s32)table);
}

/* Entry `index` (the low 16 bits) of an archive already in memory */
void *getArchiveEntry(s32 index, void *archive) {
    s32 *table = archive;

    return (void *)(table[index & 0xFFFF] + (s32)table); /* as in getFileEntry */
}

/* markCachedFiles + touchMarkedFiles: refresh the files cached before a load */
void markCachedFiles(void) {
    FileSlot *slot;
    s32 i;

    for (i = 0, slot = FILE_CACHE.slots; i < FILE_CACHE_SLOTS; i++, slot++) {
        if (slot->file == 0) {
            slot->marked = 0;
        } else {
            slot->marked = 1;
        }
    }
}

/*
 * Marks the files markCachedFiles saw as just used (the European and Japanese versions, 10 frames
 * before), so they stay cached
 */
void touchMarkedFiles(void) {
    FileSlot *slot = FILE_CACHE.slots;
#if VERSION_US
    s32 now = GFX.funcs.getTime();
#elif VERSION_EU || VERSION_JP
    s32 now = GFX.funcs.getTime() - 10;
#endif
    s32 i;

    for (i = 0; i < FILE_CACHE_SLOTS; i++, slot++) {
        if (slot->file != 0 && slot->marked != 0) {
            slot->lastUsed = now;
            slot->marked = 0;
        }
    }
}
