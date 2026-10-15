#ifndef GAME_H
#define GAME_H

#include "common.h"
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>

#include "engine/task.h"
#include "engine/heap.h"
#include "engine/gfx.h"
#include "engine/file.h"
#include "engine/pad.h"
#include "engine/random.h"
#include "engine/sound.h"
#include "engine/overlay.h"
#include "engine/text.h"
#include "engine/game_state.h"
#include "engine/memcard.h"
#include "engine/menu.h"

/* SDK functions declared here rather than from their PsyQ headers:
   strings.h and memory.h declare these without prototypes, strlen as
   unsigned */
int strlen(const char *);
char *strcpy(char *dst, const char *src);
char *strncpy(char *dst, const char *src, s32 n);
void *memcpy(void *dst, const void *src, int size);
int atoi(u8 *s); /* stdlib.h's; SHOCKTST parses its text file with these two */
int strcspn(u8 *s, char *reject);

/* The kernel's file functions (libapi.h); the sim: device is the PC's */
long open(char *name, unsigned long mode);
long read(long fd, void *buf, long n);
long write(long fd, void *buf, long n);
long close(long fd);

#endif /* GAME_H */
