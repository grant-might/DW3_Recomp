#include "common.h"
#include "engine/file.h"
#include "engine/gfx.h"

/*
 * The European version's settings, in the small data (.sdata) between
 * file/file_table.c's and system/main.c's. Every module that uses them
 * reads them through their address, so none of those defines them (in a
 * -G8 unit GCC would read them through $gp); file/file_table.c and
 * task/task.c, the modules linked between the two, never read them.
 */
#if VERSION_EU
s32 LANGUAGE = 2;
s32 NTSC_MODE = 0;
s32 SHIFT_PAL_SCREEN = 1;
#endif
