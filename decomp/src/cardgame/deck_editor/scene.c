/* STCRDDEK's scene: the mode's root task, and the screen fade */

#include "cardgame/deck_editor.h"

#define SCENE_TASK Task
#define SCENE_CHILD DeckScreen
#define SCENE_CREATE OVL_NAME(createScreen)
#define SCENE_PRIM_BUFFERS 0xF000
#define SCENE_OT_SHIFT 3
#include "shared/update_scene.inc.c"
#include "shared/start.inc.c"

#include "shared/start_fader.inc.c"
#include "shared/draw_fader.inc.c"
#include "shared/update_fader.inc.c"
#define FADER_DEPTH 0
#include "shared/create_fader.inc.c"
