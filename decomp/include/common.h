#ifndef COMMON_H
#define COMMON_H

#include "version.h"
#include "include_asm.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

#ifndef NULL
#define NULL ((void *)0)
#endif

/*
 * The length of an array that ends a struct and runs as far as each object
 * of it needs: s32 seps[FLEXIBLE]. GCC 2.8 has no flexible array members,
 * [], but takes [0], and an initializer for it; a modern GCC drops the
 * elements a [0] array is initialized with.
 */
#if __GNUC__ >= 3
#define FLEXIBLE
#else
#define FLEXIBLE 0
#endif

/*
 * A variable that an interrupt's callback writes while the game waits for
 * it in a loop: the original reads it again in each pass, as GCC 2.8.1
 * compiles the loop, where GCC 12 reads it once unless it is volatile
 * (make TOOLCHAIN=gcc)
 */
#if __GNUC__ >= 3
#define INTERRUPT_SHARED volatile
#else
#define INTERRUPT_SHARED
#endif

#endif /* COMMON_H */
