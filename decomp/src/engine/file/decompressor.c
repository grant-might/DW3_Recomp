#include "engine/game.h"

/* Decompressor method: frees its buffer */
void decompressorFree(Decompressor *task) {
    if (task->buffer != NULL) {
        HEAP.free(task->buffer);
    }
    task->buffer = NULL;
    task->bufferSize = 0;
}

/* Points the decompressor at data, which is RLEN-packed or plain */
void decompressorSetData(Decompressor *task, s32 *data) {
    task->data = data;
    if (data[0] == RLEN_MAGIC) {
        task->compressed = 1;
        task->size = data[1];
    } else {
        task->compressed = 0;
        task->size = 0;
    }
    data += 2;
    task->begin = data;
    task->src = data;
}

/* Makes the buffer big enough for the unpacked size */
void decompressorAllocBuffer(Decompressor *task) {
    if (task->size > task->bufferSize) {
        if (task->buffer != NULL) {
            HEAP.free(task->buffer);
        }
        task->buffer = HEAP.alloc(task->size, MEM_MODE);
        task->bufferSize = task->size;
    }
    task->dst = task->buffer;
}

/* Unpacks at least chunkSize bytes; back to substate 0 at the end */
void decompressorStep(Decompressor *task) {
    u8 *src = task->src;
    u8 *dst = task->dst;
    s32 total = 0;
    s32 done = 0;
    s32 n;
    s32 i;

    while (*src != 0) {
        if (*src & 0x80) {
            n = *src++ & 0x7F;
            for (i = 0; i < n; i++) {
                *dst++ = *src;
            }
            src++;
            total += n;
        } else {
            n = *src++;
            for (i = 0; i < n; i++) {
                *dst++ = *src++;
            }
            total += n;
        }
        if (total >= task->chunkSize) {
            done = 1;
            task->src = src;
            task->dst = dst;
            break;
        }
    }
    if (!done) {
        task->setSubstate(task, 0);
    }
}

/* Decompressor method: unpacks data at once; returns it unpacked (plain data as it is) */
void *decompressorRun(Decompressor *task, s32 *data) {
    task->chunkSize = 0x10000000;
    decompressorSetData(task, data);
    if (task->compressed) {
        decompressorAllocBuffer(task);
        decompressorStep(task);
        return task->buffer;
    }
    return data;
}

/* Decompressor method: starts unpacking data chunkSize bytes a frame */
void decompressorStart(Decompressor *task, s32 *data, s32 chunkSize) {
    task->chunkSize = chunkSize;
    decompressorSetData(task, data);
    if (task->compressed) {
        decompressorAllocBuffer(task);
        task->setSubstate(task, 1);
    }
}

/* Decompressor method: the unpacked data, or NULL while it is unpacking */
void *decompressorGetData(Decompressor *task) {
    if (task->substate != 0) {
        return NULL;
    }
    if (task->compressed != 0) {
        return task->buffer;
    }
    return task->data;
}

/* The decompressor task: unpacks a chunk a frame, frees the buffer at the end */
void updateDecompressor(Decompressor *task) {
    switch (task->state) {
    case 0:
    default:
        task->nextState(task);
        break;
    case 1:
        if (task->substate != 0 && task->substate == task->state) {
            decompressorStep(task);
        }
        break;
    case 2:
        break;
    case 3:
        if (task->buffer != NULL) {
            HEAP.free(task->buffer);
        }
        break;
    }
}

/* A decompressor task */
Decompressor *createDecompressor(void) {
    Decompressor *task = createTask(updateDecompressor, sizeof(Decompressor), 0);

    task->run = decompressorRun;
    task->free = decompressorFree;
    task->start = decompressorStart;
    task->getData = decompressorGetData;
    return task;
}
