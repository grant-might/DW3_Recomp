/* STGMCARD's scene: the mode's root task, and the screen fade */

#include "menus/memory_card.h"

#define SCENE_TASK MemCardScene
#define SCENE_CHILD MemCardScreen
#define SCENE_CREATE OVL_NAME(createScreen)
#define SCENE_PRIM_BUFFERS 0x5000
#define SCENE_OT_SHIFT 2
#include "shared/update_scene.inc.c"
#include "shared/start.inc.c"

#include "shared/start_fader.inc.c"
#include "shared/draw_fader.inc.c"
#include "shared/update_fader.inc.c"
#define FADER_DEPTH 0
#include "shared/create_fader.inc.c"
