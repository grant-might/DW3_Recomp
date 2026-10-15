#include "common.h"
#include "field/stage.h"

/* The color the setup function copies to FIELDSTG_state.spriteColor. It starts the stage, before
   its jump tables: in wstag924.c GCC would align them to 8 bytes after it, so
   it is a file of its own, linked first */
const CVECTOR stageColor = { 0x54, 0x67, 0x96, 0x00 };
