/* FIGHTSTG's partner view, the partner's picture beside the menus. */

#include "battle/battle_mode.h"

/* Draws the frame around the partner view */
void FIGHTSTG_drawPartnerViewFrame(PartnerView *task, FighterCamera **cameras) {
    SpriteDrawer drawer;

    initSpriteDrawer(&drawer);
    drawer.setLayerId(BATTLE_LAYER_MENUS, 0);
    drawer.setTexture(0x200, 0);
    drawer.draw(FILE_CACHE.getEntry(FILE_BATTLE_MENU << 16), 10, 246, 74);
}

/* The partner view's task: makes layer 0x1009 beside the menus, then once the
   models are there shows the player's active model on it through a new
   FighterCamera (ending the other one) and draws the frame each frame; when
   killed, takes the model off the layer and destroys it */
void FIGHTSTG_updatePartnerView(PartnerView *task, FighterCamera **cameras) {
    Models *models;
    ModelControl *control;
    FighterCamera *other;
    s32 fighter;

    switch (task->state) {
    case TASK_INIT:
    default:
        FIGHTSTG_fighterCameraRect.x = 0xD0;
        FIGHTSTG_fighterCameraRect.y = 0x4C;
        FIGHTSTG_fighterCameraRect.w = 0x64;
        FIGHTSTG_fighterCameraRect.h = 0x3C;
        task->layer = GFX.funcs.createLayer(&FIGHTSTG_fighterCameraRect, 0xC, 0x1009);
        task->layer->setOffset(task->layer, 0x102, 0x6A);
        task->layer->allocCallbacks(task->layer, 0x32);
        task->nextState(task);
        break;
    case TASK_RUN:
        if (task->substate == 0) {
            models = TASK_REGISTRY.funcs.find(BATTLE_TASK_MODELS, -1, -1);
            if (models != NULL) {
                control = models->get(models, 0);
                control->layers[1].enabled = 1;
                control->layers[1].wireframe = 0;
                control->layers[1].layerId = 0x1009;
                fighter = control->fighter;
                if (cameras[0] == NULL) {
                    cameras[0] = FIGHTSTG_createCamera(fighter, control);
                    other = cameras[1];
                } else {
                    cameras[1] = FIGHTSTG_createCamera(fighter, control);
                    other = cameras[0];
                }
                if (other != NULL) {
                    other->setState(other, 3);
                }
                task->nextSubstate(task);
            }
        }
        FIGHTSTG_drawPartnerViewFrame(task, cameras);
        break;
    case 2:
        task->nextState(task);
        break;
    case TASK_KILL:
        if (task->layer != NULL) {
            models = TASK_REGISTRY.funcs.find(BATTLE_TASK_MODELS, -1, -1);
            models->get(models, 0)->layers[1].enabled = 0;
            GFX.funcs.destroyLayer(0x1009);
        }
        break;
    }
}

/* Creates the partner view (FIGHTSTG_updatePartnerView), whose children are
   its two cameras */
PartnerView *FIGHTSTG_createPartnerView(void) {
    return createTask(FIGHTSTG_updatePartnerView, sizeof(PartnerView), 2 * sizeof(Task *));
}
