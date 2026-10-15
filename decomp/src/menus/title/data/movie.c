/* The movie player's data (movie.c), linked after libpress's data */

#include "menus/title.h"

DecEnv STDWTITL_decEnv = {0};
u_long *STDWTITL_ringBuffer = NULL;
u_short *STDWTITL_vlcTable = NULL;
u_long *STDWTITL_vlcBuffer0 = NULL;
u_long *STDWTITL_vlcBuffer1 = NULL;
u_long *STDWTITL_imageBuffer0 = NULL;
u_long *STDWTITL_imageBuffer1 = NULL;
s32 STDWTITL_movieEnded = 0;
s32 STDWTITL_movieFile = 0;
u32 STDWTITL_movieEndFrame = 0;
