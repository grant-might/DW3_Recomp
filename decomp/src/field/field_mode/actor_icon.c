/* The icon over the player's head */

#include "field/field_mode.h"

/* The icon over the player's head: plays the animation of its substate
   (FIELDSTG_iconAnims) and follows its climbs along FIELDSTG_iconPath */
void FIELDSTG_updateActorIcon(ActorIcon *task) {
    SpriteDrawer sprite;
    Vec2 pos;
    u8 (*anim)[2];
    Actor *actor;
    s32 step;
    s32 time;

    switch (task->state) {
    default:
    case TASK_INIT:
        task->nextState(task);
        /* fallthrough */
    case TASK_RUN:
        if (task->step == 0) {
            task->anim = FIELDSTG_iconAnims[task->substate];
            task->animStep = 0;
            task->animTime = 0;
            switch (task->substate) {
            case 1:
            case 3:
                SOUND.playSound(SOUND_ITEM_GET);
                break;
            }
            task->nextStep(task);
        }
        if (task->actor != NULL && task->actor->state == TASK_RUN) {
            step = task->animStep;
            time = task->animTime;
            time += GFX.funcs.getFrameTime();
            anim = task->anim;
            if (anim[step][1] < time) {
                time -= anim[step][1];
                step++;
                if (anim[step][0] == 0xFF) {
                    step = anim[step][1];
                }
                task->frame = anim[step][0];
                task->animStep = step;
            }
            task->animTime = time;
            actor = task->actor;
            switch (actor->substate) {
            case ACTOR_CLIMB_OFF_TOP:
                if (actor->climbSide != 0) {
                    pos.x = actor->tile.x + FIELDSTG_iconPath[FIELDSTG_iconPathStep].x;
                } else {
                    pos.x = actor->tile.x - FIELDSTG_iconPath[FIELDSTG_iconPathStep].x;
                }
                pos.y = actor->tile.y + FIELDSTG_iconPath[FIELDSTG_iconPathStep].y;
                if (FIELDSTG_iconPath[FIELDSTG_iconPathStep + 1].x != 0) {
                    FIELDSTG_iconPathStep++;
                }
                break;
            case ACTOR_GET_OVER_EDGE:
                if (FIELDSTG_iconPathStep == 0) {
                    FIELDSTG_iconPathStep = 0xE;
                }
                if (actor->climbSide != 0) {
                    pos.x = actor->tile.x + FIELDSTG_iconPath[FIELDSTG_iconPathStep].x;
                } else {
                    pos.x = actor->tile.x - FIELDSTG_iconPath[FIELDSTG_iconPathStep].x;
                }
                pos.y = actor->tile.y + FIELDSTG_iconPath[FIELDSTG_iconPathStep].y;
                if (FIELDSTG_iconPathStep != 1) {
                    FIELDSTG_iconPathStep--;
                }
                break;
            default:
                pos.x = actor->tile.x;
                FIELDSTG_iconPathStep = 0;
                pos.y = actor->tile.y;
                break;
            }
            initSpriteDrawer(&sprite);
            sprite.setTexture(FIELD_SPRITES_X, FIELD_SPRITES_Y);
            sprite.setLayerId(FIELD_LAYER_MAP, 2);
            sprite.draw(FILE_CACHE.getEntry(FIELD_SPRITES_FILE << 16), task->frame, pos.x, pos.y);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the player's icon, in the modes before MODE_NEW_GAME only */
ActorIcon *FIELDSTG_createActorIcon(Actor *actor) {
    ActorIcon *task;

    if (GAME.funcs.getMode() < MODE_NEW_GAME) {
        task = createTaskWithId(FIELDSTG_updateActorIcon, sizeof(ActorIcon), 0, FIELD_TASK_ICON);
        task->actor = actor;
        return task;
    }
    return NULL;
}
