/* The camera's turn around the fighters, a task that both versions have:
   the USA version in camera_task.c, the European one in battle_script.c. Each
   includes this file where its object has the task. */


/* When the player loses, turns the camera around the fighters, from the
   first fighter's view 0xB, over stage 0x17, with the battle script's task
   as the child; the European version moves on to state 2 after 240 frames,
   where the camera keeps turning */
void FIGHTSTG_updateCameraTurn(CameraTurn *task, BattleScript **children) {
    BattleCamera *camera = task->camera;
    FightStage *stage;
    Models *models;
    s32 z;

    switch (task->state) {
    case 0:
    default:
        camera = TASK_REGISTRY.funcs.find(BATTLE_TASK_CAMERA, -1, -1);
        task->camera = camera;
        camera->getFighterView(camera, 0, 0xB);
        z = FIGHTSTG_fighterView.vrz;
        FIGHTSTG_fighterView.vrz = 0;
        FIGHTSTG_fighterView.vpz -= z;
        FIGHTSTG_fighterView.tz = z;
        stage = TASK_REGISTRY.funcs.find(BATTLE_TASK_STAGE, -1, -1);
        stage->setStage(stage, 0x17, 0x1E, 0x1E);
        models = TASK_REGISTRY.funcs.find(BATTLE_TASK_MODELS, -1, -1);
        models->get(models, 0x10)->layers[0].enabled = 0;
        FIGHTSTG_battle.setSpeed(2);
        *children = FIGHTSTG_createBattleScript();
        (*children)->index = 3;
        (*children)->enemy = 0;
        task->nextState(task);
        /* fallthrough */
    case 1:
#if VERSION_EU || VERSION_JP
        task->substate += GFX.funcs.getFrameTime();
        if (task->substate >= 240) {
            task->setState(task, 2);
        }
        /* fallthrough */
    case 2:
#endif
        FIGHTSTG_fighterView.rot.vy += GFX.funcs.getFrameTime() * 2;
        camera->set(camera, &FIGHTSTG_fighterView);
        break;
    case 3:
        FIGHTSTG_battle.setSpeed(0);
        break;
#if VERSION_US
    case 2:
        break;
#endif
    }
}

/* Starts the camera's turn around the fighters */
CameraTurn *FIGHTSTG_startCameraTurn(void) {
    return createTask(FIGHTSTG_updateCameraTurn, sizeof(CameraTurn), sizeof(Task *));
}
