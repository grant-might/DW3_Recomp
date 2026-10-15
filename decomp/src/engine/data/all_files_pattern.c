#include "common.h"
#include "engine/memcard.h"

/*
 * In the small data (.sdata) between system/main.c's and text/cursor.c's.
 * memcard/memcard.c reads STR_ALL_FILES through its address, so it is not
 * its small data; and 16 bytes are too many for one small variable, so the
 * original had more than one here: "*", 8 bytes of zeros and an unread
 * grey (128, 128, 128). The attribute puts the 16 bytes where they were.
 */
char STR_ALL_FILES[16] __attribute__((section(".sdata"))) = "*\0\0\0" "\0\0\0\0\0\0\0\0" "\x80\x80\x80";
