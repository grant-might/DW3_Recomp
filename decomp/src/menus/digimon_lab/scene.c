/* STGDGLAB's scene: the mode's root task, and the screen fade */

#include "menus/digimon_lab.h"

#define SCENE_TASK Task
#define SCENE_CHILD Lab
#define SCENE_CREATE OVL_NAME(createLab)
#define SCENE_PRIM_BUFFERS 0xF000
#define SCENE_OT_SHIFT 3
#include "shared/update_scene.inc.c"

/* Starts the mode: creates its scene task */
Task *STGDGLAB_createScene(void) {
    return createTask(STGDGLAB_updateScene, sizeof(Task), sizeof(Lab *));
}

#include "shared/start_fader.inc.c"
#include "shared/draw_fader.inc.c"
#include "shared/update_fader.inc.c"
#define FADER_DEPTH 6
#include "shared/create_fader.inc.c"
