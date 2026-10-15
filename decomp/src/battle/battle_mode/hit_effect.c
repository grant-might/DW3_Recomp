/* FIGHTSTG's hit effect, the spark and script of a hit. */

#include "battle/battle_mode.h"

/* where FIGHTSTG_updateHitEffect's effect 0x33 is: a screen position, in
   front (z 0x7FFF, see FIGHTSTG_drawSpriteAnim) */
const SVECTOR FIGHTSTG_hitEffectPos = { 0, 120, 0x7FFF, 0 };

/* Loads effect 0x33, then turns the camera to the enemy's view, plays the
   effect with a sound and, 10 frames on, the hit's or the knockout's script */
void FIGHTSTG_updateHitEffect(HitEffect *task, HitEffectChildren *children) {
    BattleCamera *camera = task->camera;

    switch (task->state) {
    case 0:
    default:
        switch (task->substate) {
        case 0:
        default:
            task->models = TASK_REGISTRY.funcs.find(BATTLE_TASK_MODELS, -1, -1);
            task->camera = TASK_REGISTRY.funcs.find(BATTLE_TASK_CAMERA, -1, -1);
            task->nextSubstate(task);
            /* fallthrough */
        case 1:
            switch (task->step) {
            case 0:
            default:
                FIGHTSTG_findEffectSheet(0x33, &task->effectImages, &task->sheet, &task->texPos);
                task->nextStep(task);
                /* fallthrough */
            case 1:
                if (FILE_CACHE.isLoading(task->sheet >> 16) == 0) {
                    task->nextState(task);
                }
                break;
            }
            break;
        }
        break;
    case 1:
        switch (task->substate) {
        case 0:
            camera->set(camera, camera->getEnemyView(camera));
            children->effect = FIGHTSTG_startSpriteEffect(0x33, &FIGHTSTG_hitEffectPos);
            SOUND.playSound(SOUND_COMCD103);
            task->nextSubstate(task);
            /* fallthrough */
        case 1:
            task->counter += GFX.funcs.getFrameTime();
            if (task->counter < 10) {
                break;
            }
            children->script = FIGHTSTG_createBattleScript();
            children->script->enemy = 0;
            children->script->index = task->result + 1;
            children->script->unk74 = task->unk5C;
            task->nextSubstate(task);
            /* fallthrough */
        case 2:
            if (children->script == NULL) {
                task->setState(task, 3);
            }
            break;
        }
        break;
    case 2:
    case 3:
        break;
    }
}

/* Starts the effect of a hit on the player's fighter (result 1) or of its
   knockout (2) */
HitEffect *FIGHTSTG_startHitEffect(s32 result, s32 arg1) {
    HitEffect *task = createTask(FIGHTSTG_updateHitEffect, sizeof(HitEffect), sizeof(HitEffectChildren));

    task->result = result;
    task->unk5C = arg1;
    return task;
}
