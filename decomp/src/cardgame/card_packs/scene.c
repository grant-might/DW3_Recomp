/* STCRDSHP's scene: the mode's root task */

#include "cardgame/card_packs.h"

#define SCENE_TASK Task
#define SCENE_CHILD CardShop
#define SCENE_CREATE OVL_NAME(createShop)
#define SCENE_PRIM_BUFFERS 0xF000
#define SCENE_OT_SHIFT 3
#include "shared/update_scene.inc.c"
#include "shared/start.inc.c"
