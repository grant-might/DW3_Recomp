#include "engine/game.h"

/* Frees a heap block, merging it with the free blocks around it */
void freeMem(void *ptr) {
    MemBlock *block = (MemBlock *)ptr - 1; /* the header just before the data */
    MemBlock *prev;
    MemBlock *next;

    if (ptr != NULL) {
        prev = block->prev;
        next = block->next;
        block->tag = MEM_FREE;
        if (next->tag == MEM_FREE) {
            block->next = next->next;
            next->next->prev = block;
        }
        if (prev->tag == MEM_FREE) {
            prev->next = block->next;
            block->next->prev = prev;
        }
    }
}

/* HEAP.nop: empty, and nothing calls it */
void heapNop(void) {
}

/* Frees every heap block with tag `tag` */
void freeMemByTag(s32 tag) {
    MemBlock *block;

    for (block = HEAP.first; block->tag != MEM_END; block = block->next) {
        if (block->tag == tag) {
            freeMem(block + 1);
        }
    }
}

/* Makes the heap one free block from HEAP_START, followed by the end marker */
void initHeap(void) {
    MemBlock *start;
    MemBlock *last;

    HEAP.end = HEAP_END;
    last = HEAP_END - 1;
    start = HEAP_START;
    HEAP.first = start;
    HEAP.size = (u8 *)HEAP_END - (u8 *)start;
    start->prev = start;
    start->next = last;
    start->tag = MEM_FREE;
    last->prev = start;
    last->tag = MEM_END;
    last->next = HEAP.end;
}

/* Clears `size` bytes, a word at a time when size is a multiple of 4 */
void zeroMem(void *dst, s32 size) {
    s32 i;

    if (size & 3) {
        u8 *p = dst;

        for (i = 0; i < size; i++) {
            *p++ = 0;
        }
    } else {
        s32 *p = dst;

        size >>= 2;
        for (i = 0; i < size; i++) {
            *p++ = 0;
        }
    }
}

/* Sets `count` bytes to `value` */
void fillMem(s8 *dst, s8 value, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        *dst++ = value;
    }
}

/* First fit from the start of the heap */
void *tryAllocMem(u32 size, s32 tag) {
    MemBlock *b;
    MemBlock *new;
    u32 avail;
    u32 splitSize;

    size = (size + 3) >> 2 << 2;
    splitSize = size + 20;
    for (b = HEAP.first; b->tag != MEM_END; b = b->next) {
        if (b->tag == MEM_FREE) {
            avail = (u8 *)b->next - (u8 *)b - sizeof(MemBlock);
            if (avail >= size) {
                if (avail > splitSize) {
                    new = (MemBlock *)((u8 *)b + size + sizeof(MemBlock));
                    new->prev = b;
                    new->next = b->next;
                    new->tag = MEM_FREE;
                    b->next->prev = new;
                    b->next = new;
                }
                b->tag = tag;
                return b + 1;
            }
        }
    }
    return NULL;
}

/* First fit from the end of the heap */
void *tryAllocMemHigh(u32 size, s32 tag) {
    MemBlock *b;
    MemBlock *prev;
    MemBlock *new;
    u32 avail;

    size = ((size + 3) >> 2 << 2) + sizeof(MemBlock);
    for (b = HEAP.end - 1; HEAP.first != b; b = b->prev) {
        prev = b->prev;
        if (prev->tag == MEM_FREE) {
            avail = (u8 *)b - (u8 *)prev;
            if (size == avail) {
                new = prev;
                new->tag = tag;
                return new + 1;
            }
            if (size < avail) {
                new = (MemBlock *)((u8 *)b - size);
                new->prev = prev;
                new->next = b;
                new->tag = tag;
                b->prev->next = new;
                b->prev = new;
                return new + 1;
            }
        }
    }
    return NULL;
}

/* Evicts cached files until the allocation fits */
void *allocMem(s32 size, s32 tag) {
    void *ptr;

    while ((ptr = tryAllocMem(size, tag)) == NULL) {
        FILE_CACHE.evictOldest();
    }
    return ptr;
}

/* The block is returned in v0, left there by tryAllocMemHigh */
void allocMemHigh(s32 size, s32 tag) {
    while (tryAllocMemHigh(size, tag) == 0) {
        FILE_CACHE.evictOldest();
    }
}

/* allocMem, cleared */
void *allocMemZeroed(s32 size, s32 tag) {
    void *ret = allocMem(size, tag);

    zeroMem(ret, size);
    return ret;
}

/* Keeps a block alive across mode changes, or hands it back to the mode */
void lockMem(void *ptr, s32 lock) {
    MemBlock *block = (MemBlock *)ptr - 1; /* the header just before the data */

    if (lock) {
        block->tag = MEM_LOCKED;
    } else {
        block->tag = MEM_MODE;
    }
}

/* The functions that return what a call left in v0 (allocMemHigh, findTask) and the ones that take
   other types are cast to what the tables give their callers */
Heap HEAP = {
    0,
    NULL,
    NULL,
    initHeap,
    freeMem,
    freeMemByTag,
    allocMem,
    (void *(*)(s32, s32))allocMemHigh,
    allocMemZeroed,
    zeroMem,
    (void (*)(void *, s32, s32))fillMem,
    lockMem,
    heapNop,
};
