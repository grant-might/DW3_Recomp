/* STITSHOP's scene: the mode's root task, and the screen fade */

#include "menus/item_shop.h"

#define SCENE_TASK Task
#define SCENE_CHILD ItemShop
#define SCENE_CREATE OVL_NAME(createShop)
#define SCENE_PRIM_BUFFERS 0x14000
#define SCENE_OT_SHIFT 3
#include "shared/update_scene.inc.c"
#include "shared/start.inc.c"

#include "shared/start_fader.inc.c"
#include "shared/draw_fader.inc.c"
#include "shared/update_fader.inc.c"
#define FADER_DEPTH 6
#include "shared/create_fader.inc.c"
