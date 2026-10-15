/*
 * The PS-X EXE header: the BIOS loads the file's .text, everything after
 * these 0x800 bytes, at its address and starts it at the entry point. The
 * addresses and the size are symbols, so they follow the code (splat would
 * write them as numbers: main.yaml doesn't extract the header).
 */

.section .data

.ascii "PS-X EXE"                   /* magic number */
.word 0                             /* .text vram address */
.word 0                             /* .data vram address */
.word __SN_ENTRY_POINT              /* initial PC: crt0, PsyQ's startup */
.word 0                             /* initial $gp, which crt0 sets */
.word main_VRAM                     /* .text start */
.word EXE_TEXT_SIZE                 /* .text size (undefined_syms.txt) */
.word 0                             /* .data start */
.word 0                             /* .data size */
.word 0                             /* .bss start */
.word 0                             /* .bss size */
.word 0x801FFFF0                    /* initial $sp and $fp: the top of the RAM */
.word 0                             /* initial $sp and $fp offset */
.word 0, 0, 0, 0, 0                 /* reserved */
.if VERSION_US
.ascii "Sony Computer Entertainment Inc. for North America area"
.elseif VERSION_EU
.ascii "Sony Computer Entertainment Inc. for Europe area"
.elseif VERSION_JP
.ascii "Sony Computer Entertainment Inc. for Japan area"
.endif
.org 0x800
