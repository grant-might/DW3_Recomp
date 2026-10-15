#ifndef DW3_RANDOM_H
#define DW3_RANDOM_H

/* Random numbers (system/random.c) */

#include "common.h"

/* The entries of RANDOM_TABLE, each 0 to RANDOM_COUNT - 1 */
#define RANDOM_COUNT 0x1000

/* Random numbers, walked in order through RANDOM_TABLE */
typedef struct Random {
    /* 0x0 */ s32 index; /* the entry returned last */
    /* 0x4 */ void (*seed)(s32 seed);
    /* 0x8 */ s32 (*next)(void); /* 0 to RANDOM_COUNT - 1 */
} Random;

void seedRandom(s32 seed);
s32 random(void);

extern Random RANDOM;
extern u16 RANDOM_TABLE[RANDOM_COUNT];

#endif /* DW3_RANDOM_H */
