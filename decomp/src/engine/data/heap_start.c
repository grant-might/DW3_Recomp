#include "common.h"
#include "engine/heap.h"

/*
 * The first of the executable's small data (.sdata), where $gp points.
 * system/heap.c reads it through its address: defined there, a -G8 unit,
 * GCC would read it through $gp. Its owner is linked before game/items,
 * whose small data come next.
 */
MemBlock *HEAP_START = HEAP_BASE;
