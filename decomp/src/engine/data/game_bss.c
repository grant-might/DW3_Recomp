#include "common.h"

/*
 * The executable's .bss that no module can hold. The modules leave these
 * variables uninitialized (static in -G8 units), so maspsx emits them as
 * common symbols, and the original's linker laid the commons out 8 bytes
 * apart in link order; GNU ld would lay them out its own way, so these
 * definitions, in a unit whose commons maspsx turns into .bss (see the
 * Makefile), fix the original's addresses. CD_SECTOR_HEADER, the .bss of
 * file/cd_reader.c, stays with them: defining it there needs maspsx to turn
 * that unit's commons into its own sections, CD_MODE with them, which would
 * take CD_MODE out of the commons around it.
 */

/*
 * 2MBYTE.OBJ's .bss, where its startup code, crt0, keeps $ra across InitHeap;
 * the second word is unused. Defined here, at the start of the .bss, so its
 * bytes stay in a report unit.
 */
s32 CRT0_SAVED_RA[2];
struct FieldMenuChoice FIELD_MENU_CHOICE; /* menu/field_menu.c's */
u8 CD_MODE[8];            /* file/cd_reader.c's */
s32 FLIP_PENDING[2];      /* gfx/display.c's */
s32 CARD_DRAWER[2];       /* gfx/card_drawer.c's */
s32 SPRITE_DRAWER[2];     /* gfx/sprite_drawer.c's */
s32 TEXT_TOOLS[2];        /* text/text_tools.c's */
s32 TIM_LOADER[2];        /* gfx/tim_loader.c's */
struct CdSectorHeader CD_SECTOR_HEADER; /* file/cd_reader.c's */

/*
 * The two structs above, completed (and checked against the declarations)
 * only now: GCC emits the commons in the order of their first declaration,
 * and the header's would come before the others.
 */
#include "engine/game.h"
