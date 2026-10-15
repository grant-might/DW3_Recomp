/* The questions' panel opens and closes as the menus' panels do, with
   their code */

#include "field/field_mode.h"

#define START_FADE OVL_NAME(startTween)
#include "shared/start_fade.inc.c"
#define UPDATE_FADE OVL_NAME(updateTween)
#include "shared/update_fade.inc.c"
