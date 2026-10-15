#ifndef FIGHTSTG_TASK_H
#define FIGHTSTG_TASK_H

/* FIGHTSTG's battle children (BattleChild): the tasks an action or a menu
   keeps, by kind. */

#include "battle/battle_mode/types.h"
#include "battle/battle_mode/camera.h"
#include "battle/battle_mode/effect.h"
#include "battle/battle_mode/battle.h"
#include "battle/battle_mode/action.h"
#include "battle/battle_mode/menu.h"

/* A turn's or a counterattack's one child, and what WFIGHTMN's battle menu
   waits for (NULL when it is done): a message box, or what plays an action */
union BattleChild {
    Task *task;
    BattleMessageBox *message; /* FIGHTSTG_createMessage */
    BattleScript *script; /* a technique's (FIGHTSTG_createBattleScript) */
    Entrance *entrance; /* a fighter's (FIGHTSTG_startEntrance) */
    DigimonChange *change; /* a partner's Digimon change (FIGHTSTG_startDigimonChange) */
    FirstTech *attack; /* FIGHTSTG_startFirstTech */
    TechAction *tech; /* a technique or an item (FIGHTSTG_startTechAction) */
    BattleItem *item; /* FIGHTSTG_startItem */
    EnemyAttack *enemyAttack; /* an attack on the player's fighter (FIGHTSTG_startEnemyAttack) */
    HitEffect *hitEffect; /* effect 0x33, then a script (FIGHTSTG_startHitEffect) */
    EnemyTurn *enemyTurn; /* FIGHTSTG_startEnemyTurn */
    OneHpTurn *oneHpTurn; /* BATTLE_KIND_FINAL_SECOND's enemy turn (FIGHTSTG_startOneHpTurn) */
    CameraTurn *cameraTurn; /* when the player loses (FIGHTSTG_startCameraTurn) */
    CameraShots *cameraShots; /* the European version's (FIGHTSTG_startCameraShots) */
    ScreenFade *fade; /* FIGHTSTG_createFader */
    Counterattack *counter; /* a counterattack (FIGHTSTG_startCounterattack) */
    ActionEvents *events; /* the queued events (FIGHTSTG_startActionEvents) */
};

#endif /* FIGHTSTG_TASK_H */
