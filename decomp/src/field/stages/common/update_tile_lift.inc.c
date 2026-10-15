/*
 * The lift: moves the map objects of animations 3 and 2 (shown only while it
 * moves) and the player 0x7F up or down each time an event sets TASK_DONE,
 * shaken by the stage's tileLiftShake before and after
 */
void updateTileLift(Lift *task) {
    StageTile *object;
    StageTile *tile0;
    StageTile *tile1;
    Actor *player;
    s32 d;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        for (object = FIELDSTG_state.objects; object->margin != 0; object++) {
            switch (object->anim) {
            case 2:
                task->tiles[1] = object;
                task->homeY[1] = object->y;
                if (task->raised) {
                    object->y -= 0x7F;
                }
                object->visible = 0;
                break;
            case 3:
                task->tiles[0] = object;
                task->homeY[0] = object->y;
                if (task->raised) {
                    object->y -= 0x7F;
                }
                object->visible = 1;
                break;
            }
        }
        task->raised = 0;
        break;
    case TASK_RUN:
        break;
    case TASK_DONE:
        tile0 = task->tiles[0];
        tile1 = task->tiles[1];
        player = TASK_REGISTRY.funcs.find(FIELD_TASK_ACTOR, -1, 0);
        switch (task->substate) {
        case 0:
        default:
            tile1->visible = 1;
            task->timer = 0;
            task->y[0] = tile0->y;
            task->y[1] = tile1->y;
            task->playerY = player->pos.y;
            SOUND.playSound(SOUND_SWITCH01);
            task->nextSubstate(task);
            break;
        case 1:
            task->timer += GFX.funcs.getFrameTime();
            if (task->timer >= 0x1E) {
                task->shake = 0;
                task->nextSubstate(task);
                SOUND.playSound(SOUND_ELEVATER);
            }
            break;
        case 2:
        case 4:
            d = tileLiftShake[task->shake];
            if (d != 0x3E8) {
                tile0->y = task->y[0] + d;
                tile1->y = task->y[1] + d;
                player->pos.y = task->playerY + d;
                task->shake++;
            } else {
                task->nextSubstate(task);
                task->shake = 0;
            }
            break;
        case 3:
            if (++task->shake >= 0xFE) {
                if (task->raised) {
                    tile0->y = task->homeY[0];
                    tile1->y = task->homeY[1];
                    player->pos.y = task->playerY + 0x7F00;
                } else {
                    tile0->y = task->homeY[0] - 0x7F;
                    tile1->y = task->homeY[1] - 0x7F;
                    player->pos.y = task->playerY - 0x7F00;
                }
                task->y[0] = tile0->y;
                task->y[1] = tile1->y;
                task->playerY = player->pos.y;
                task->nextSubstate(task);
                task->shake = 0;
            } else if (task->shake & 1) {
                if (task->raised) {
                    tile0->y++;
                    tile1->y++;
                    player->pos.y += 0x100;
                } else {
                    tile0->y--;
                    tile1->y--;
                    player->pos.y -= 0x100;
                }
            }
            break;
        case 5:
            tile1->visible = 0;
            task->setState(task, TASK_RUN);
            task->raised ^= 1;
            break;
        }
        break;
    case TASK_KILL:
        break;
    }
}
