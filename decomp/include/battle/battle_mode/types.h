#ifndef FIGHTSTG_TYPES_H
#define FIGHTSTG_TYPES_H

/* FIGHTSTG's shared declarations: the headers it builds on, the ids its
   tasks register with, and the types the other headers point to first. */

#include "engine/game.h"
#include <libgs.h>
#include "engine/menu.h"
#include "engine/file.h"

/* The types that the headers point to before they are defined */
typedef struct BattleScript BattleScript;
typedef union BattleChild BattleChild;

/* The ids that the battle's tasks register with, for the others to find them
   (TASK_REGISTRY.funcs.find) */
#define BATTLE_TASK_MENU 0xC /* WFIGHTMN's battle menu */
#define BATTLE_TASK_PLAYER_TURN 0xE
#define BATTLE_TASK_MODEL 0x11 /* a model (FIGHTSTG_updateModel) */
#define BATTLE_TASK_CAMERA 0x12
#define BATTLE_TASK_LIGHTS 0x13
#define BATTLE_TASK_MODELS 0x14 /* the fighters' */
#define BATTLE_TASK_STAGE 0x15

/* The battle's layers, over the screen's (SCREEN_LAYER) */
#define BATTLE_LAYER_CAMERA 0x1001 /* the battle camera's and the lights' */
#define BATTLE_LAYER_STAGE 0x1002
#define BATTLE_LAYER_WIREFRAME 0x1003 /* the models' wireframe copies, in a Digimon change */
#define BATTLE_LAYER_MODELS 0x1004
#define BATTLE_LAYER_MENUS 0x1005 /* the HUD, the menus and the messages */
#define BATTLE_LAYER_FRONT 0x1006 /* the fades, the flashes and the sprite effects over all */

/* The body of WFIGHTMN's and WFIGHTTS's layer setups: the display and the
   battle's layers on SCREEN (a RECT), the front one with FRONTCALLBACKS
   callbacks. A macro, as an inline's pointer to SCREEN is taken before the
   calls, which moves its load */
#define FIGHTSTG_CREATE_LAYERS(screen, frontCallbacks)                          \
    Layer *layer;                                                               \
                                                                                \
    GFX.funcs.reset();                                                          \
    GFX.funcs.allocPrimBuffers(0x19000);                                        \
    GFX.funcs.setDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);                \
    layer = GFX.funcs.createLayer(&(screen), 1, SCREEN_LAYER);                  \
    layer->setOffset(layer, 0xA0, 0x78);                                        \
    layer = GFX.funcs.createLayer(&(screen), 1, BATTLE_LAYER_CAMERA);           \
    layer->setOffset(layer, 0xA0, 0x78);                                        \
    layer->allocCallbacks(layer, 100);                                          \
    layer = GFX.funcs.createLayer(&(screen), 8, BATTLE_LAYER_STAGE);            \
    layer->setOffset(layer, (screen).w / 2, (screen).h / 2);                    \
    layer->allocCallbacks(layer, 40);                                           \
    layer = GFX.funcs.createLayer(&(screen), 1, BATTLE_LAYER_WIREFRAME);        \
    layer->setOffset(layer, 0xA0, 0x78);                                        \
    layer->allocCallbacks(layer, 100);                                          \
    layer = GFX.funcs.createLayer(&(screen), 12, BATTLE_LAYER_MODELS);          \
    layer->setOffset(layer, 0xA0, 0x78);                                        \
    layer->allocCallbacks(layer, 100);                                          \
    layer = GFX.funcs.createLayer(&(screen), 1, BATTLE_LAYER_MENUS);            \
    layer->setOffset(layer, 0, 0);                                              \
    layer = GFX.funcs.createLayer(&(screen), 1, BATTLE_LAYER_FRONT);            \
    layer->setOffset(layer, 0, 0);                                              \
    layer->allocCallbacks(layer, frontCallbacks)

#endif /* FIGHTSTG_TYPES_H */
