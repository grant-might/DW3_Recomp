/* A training's Digimon (TrainActor), animated from an image set */

#include "menus/gym.h"

/* Makes the Digimon's sprite from its loaded image set, and the effect's from
   set 8, when the file has one */
static inline void STGTRAIN_setUpActor(TrainActor *actor, TrainActorSprites *sprites) {
    s32 *pos;
    s32 filePos;
    s32 x;
    s32 y;

    filePos = STGTRAIN_state.getFilePos(actor->file);
    STGTRAIN_state.readSet(actor->set);
    /* the match depends on pos set here, after the two calls: set
       before them it lives too long, and x takes its register */
    pos = actor->pos;
    STGTRAIN_state.loadSet(actor->set, pos);
    if (sprites->sprite == NULL) {
        sprites->sprite = STGTRAIN_createSprite();
    }
    x = filePos & 0xFFFF;
    sprites->sprite->setBank(sprites->sprite, STGTRAIN_state.getBank(actor->set),
                             STGTRAIN_state.getBankOffset(actor->set));
    y = (u32)filePos >> 16;
    sprites->sprite->setAnim(sprites->sprite, STGTRAIN_state.getAnim(actor->set, actor->anim));
    sprites->sprite->setPos(sprites->sprite, x, y);
    sprites->sprite->setImagePos(sprites->sprite, actor->pos[0], actor->pos[1]);
    sprites->sprite->setClutPos(sprites->sprite, actor->pos[2], actor->pos[3]);
    sprites->sprite->setLayer(sprites->sprite, actor->layerId, actor->depth);
    if (STGTRAIN_state.readSet(8) != 0) {
        actor->savedX = actor->pos[0];
        actor->pos[0] = actor->pos[0] + 0x80;
        STGTRAIN_state.loadSet(8, pos);
        actor->pos[0] = actor->savedX;
        if (sprites->effect == NULL) {
            sprites->effect = STGTRAIN_createSprite();
        }
        sprites->effect->setBank(sprites->effect, STGTRAIN_state.getBank(8), STGTRAIN_state.getBankOffset(8));
        sprites->effect->setAnim(sprites->effect, STGTRAIN_state.getAnim(8, actor->anim));
        sprites->effect->setPos(sprites->effect, x, y);
        sprites->effect->setImagePos(sprites->effect, actor->pos[0], actor->pos[1]);
        sprites->effect->setClutPos(sprites->effect, actor->pos[2], actor->pos[3]);
        if (STGTRAIN_state.getFileEffectSameDepth(actor->file) == 0) {
            sprites->effect->setLayer(sprites->effect, actor->layerId, actor->depth - 1);
        } else {
            sprites->effect->setLayer(sprites->effect, actor->layerId, actor->depth);
        }
    } else if (sprites->effect != NULL) {
        sprites->effect->setState(sprites->effect, TASK_KILL);
    }
    actor->result = -1;
}

/* Mode 2: grows or shrinks the Digimon by scaleStep a frame, about its
   middle, until it is whole or gone */
static inline void STGTRAIN_scaleActor(TrainActor *actor, TrainActorSprites *sprites) {
    actor->scale += actor->scaleStep;
    if (actor->scaleStep > 0) {
        if (actor->scale > ONE) {
            actor->scale = ONE;
            actor->scaleStep = 0;
            actor->mode = 1;
        }
    } else if (actor->scale < 0) {
        actor->scale = 0;
        actor->scaleStep = 0;
        actor->mode = 1;
    }
    if (sprites->sprite != NULL) {
        sprites->sprite->setScale(sprites->sprite, actor->scale, actor->scale, ONE);
        sprites->sprite->setPivot(sprites->sprite, 0xCF, 0x7F);
    }
    if (sprites->effect != NULL) {
        sprites->effect->setScale(sprites->effect, actor->scale, actor->scale, ONE);
        sprites->effect->setPivot(sprites->effect, 0xCF, 0x7F);
    }
}

/* Whether the Digimon's animation (and the effect's) has ended */
static inline s32 STGTRAIN_actorAnimDone(TrainActorSprites *sprites) {
    if (sprites->effect != NULL) {
        return sprites->sprite->getFlags(sprites->sprite) & sprites->effect->getFlags(sprites->effect);
    }
    return sprites->sprite->getFlags(sprites->sprite);
}

/* Pauses the Digimon and its effect on their last frame and goes idle
   (mode 1) */
static inline void STGTRAIN_stopActor(TrainActor *actor, TrainActorSprites *sprites) {
    if (sprites->sprite != NULL) {
        sprites->sprite->setPaused(sprites->sprite, 1);
    }
    if (sprites->effect != NULL) {
        sprites->effect->setPaused(sprites->effect, 1);
    }
    actor->anim = 0;
    actor->setSubstate(actor, 0);
    actor->mode = 1;
}

/* Mode 4: plays the training's two animations, then the one of whether it
   worked, which the chance decides, then stops */
static inline void STGTRAIN_playTraining(TrainActor *actor, TrainActorSprites *sprites) {
    s32 worked;

    if (!STGTRAIN_actorAnimDone(sprites)) {
        return;
    }
    if (actor->substate != 0) {
        STGTRAIN_stopActor(actor, sprites);
        return;
    }
    actor->anim++;
    if (actor->anim >= 2) {
        worked = actor->chance >= RANDOM.next() % 100;
        if (worked == 1) {
            if (sprites->sprite != NULL) {
                sprites->sprite->setAnim(sprites->sprite, STGTRAIN_state.getAnim(actor->set, 2));
            }
            if (sprites->effect != NULL) {
                sprites->effect->setAnim(sprites->effect, STGTRAIN_state.getAnim(8, 2));
            }
            /* and on a 1 here: CSE puts worked's register
               in, while worked itself leaves it a copy */
            actor->result = 1;
        } else {
            if (sprites->sprite != NULL) {
                sprites->sprite->setAnim(sprites->sprite, STGTRAIN_state.getAnim(actor->set, 3));
            }
            if (sprites->effect != NULL) {
                sprites->effect->setAnim(sprites->effect, STGTRAIN_state.getAnim(8, 3));
            }
            actor->result = 0;
        }
        actor->nextSubstate(actor);
    } else {
        if (sprites->sprite != NULL) {
            sprites->sprite->setAnim(sprites->sprite, STGTRAIN_state.getAnim(actor->set, actor->anim));
        }
        if (sprites->effect != NULL) {
            sprites->effect->setAnim(sprites->effect, STGTRAIN_state.getAnim(8, actor->anim));
        }
        actor->result = -1;
    }
}

/* Mode 8: plays the end of the training, worked (animation 4) or not (5),
   then stops; without that animation the Digimon goes idle at once */
static inline void STGTRAIN_playTrainingEnd(TrainActor *actor, TrainActorSprites *sprites) {
    TrainAnim *anim;

    if (actor->substate == 0) {
        if (actor->result != 0) {
            actor->anim = 4;
        } else {
            actor->anim = 5;
        }
        if (sprites->sprite != NULL) {
            anim = STGTRAIN_state.getAnim(actor->set, actor->anim);
            if (anim != NULL) {
                sprites->sprite->setAnim(sprites->sprite, anim);
            } else {
                actor->mode = 1;
            }
        }
        if (sprites->effect != NULL) {
            anim = STGTRAIN_state.getAnim(8, actor->anim);
            if (anim != NULL) {
                sprites->effect->setAnim(sprites->effect, anim);
            } else {
                actor->mode = 1;
            }
        }
        actor->substate++;
    } else if (STGTRAIN_actorAnimDone(sprites)) {
        STGTRAIN_stopActor(actor, sprites);
    }
}

/*
 * A training's Digimon (TrainActor): once its file is loaded and its
 * positions set, makes its sprite (and the effect of set 8); then grows or
 * shrinks (mode 2), plays the training (mode 4: two animations, then the
 * chance decides between the worked and failed ones) or its end (mode 8).
 */
void STGTRAIN_updateActor(TrainActor *actor, TrainActorSprites *sprites) {
    switch (actor->state) {
    case TASK_INIT:
    default:
        if (FILE_CACHE.isLoading(STGTRAIN_state.getFileId(actor->file)) == 0 && actor->posSet != 0 &&
            actor->clutSet != 0) {
            actor->nextState(actor);
            STGTRAIN_setUpActor(actor, sprites);
        }
        break;
    case TASK_RUN:
        if (actor->scaleChanged != 0) {
            if (sprites->sprite != NULL) {
                sprites->sprite->setScale(sprites->sprite, actor->scale, actor->scale, ONE);
            }
            if (sprites->effect != NULL) {
                sprites->effect->setScale(sprites->effect, actor->scale, actor->scale, ONE);
            }
            actor->scaleChanged = 0;
        }
        switch (actor->mode) {
        case 1:
            break;
        case 2:
            STGTRAIN_scaleActor(actor, sprites);
            break;
        case 4:
            STGTRAIN_playTraining(actor, sprites);
            break;
        case 8:
            STGTRAIN_playTrainingEnd(actor, sprites);
            break;
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Pauses the Digimon's sprite and effect */
void STGTRAIN_pauseActor(TrainActor *actor) {
    TrainActorSprites *sprites = actor->children;

    if (sprites->sprite != NULL) {
        sprites->sprite->setPaused(sprites->sprite, 1);
    }
    if (sprites->effect != NULL) {
        sprites->effect->setPaused(sprites->effect, 1);
    }
}

/* Plays the training on (mode 4), unpausing the sprite and effect */
void STGTRAIN_playActor(TrainActor *actor) {
    TrainActorSprites *sprites = actor->children;

    if (sprites->sprite != NULL) {
        sprites->sprite->setPaused(sprites->sprite, 0);
    }
    if (sprites->effect != NULL) {
        sprites->effect->setPaused(sprites->effect, 0);
    }
    actor->mode = 4;
}

/* Sets where the Digimon's images go in VRAM */
void STGTRAIN_setActorPos(TrainActor *actor, s32 x, s32 y) {
    actor->pos[0] = x;
    actor->pos[1] = y;
    actor->posSet = 1;
}

/* Sets where the Digimon's CLUTs go in VRAM */
void STGTRAIN_setActorClutPos(TrainActor *actor, s32 x, s32 y) {
    actor->pos[2] = x;
    actor->pos[3] = y;
    actor->clutSet = 1;
}

/* Sets the chance that the training works */
void STGTRAIN_setActorChance(TrainActor *actor, s32 chance) {
    actor->chance = chance;
}

/* Makes the Digimon grow from nothing (mode 2) */
void STGTRAIN_growActor(TrainActor *actor) {
    actor->scaleStep = 0x199;
    actor->scale = 0;
    actor->mode = 2;
}

/* Makes the Digimon shrink to nothing (mode 2) */
void STGTRAIN_shrinkActor(TrainActor *actor) {
    actor->scale = ONE;
    actor->scaleStep = -0x333;
    actor->mode = 2;
}

/* Scales the Digimon (0x1000 is its size) */
void STGTRAIN_setActorScale(TrainActor *actor, s32 scale) {
    actor->scaleChanged = 1;
    actor->scale = scale;
}

/* Whether the training worked, once it is done (mode bit 0), else -1 */
s32 STGTRAIN_getActorResult(TrainActor *actor) {
    if (actor->mode & 1) {
        return actor->result;
    }
    return -1;
}

/* Plays the training's end (mode 8) */
void STGTRAIN_endActor(TrainActor *actor) {
    actor->mode = 8;
    actor->substate = 0;
}

/* Creates an animated sprite of an image set */
TrainActor *STGTRAIN_createActor(s32 set, s32 file, s32 layerId, s32 depth) {
    TrainActor *actor = createTask(STGTRAIN_updateActor, sizeof(TrainActor), sizeof(TrainActorSprites));

    actor->setPos = STGTRAIN_setActorPos;
    actor->setClutPos = STGTRAIN_setActorClutPos;
    actor->setChance = STGTRAIN_setActorChance;
    actor->grow = STGTRAIN_growActor;
    actor->shrink = STGTRAIN_shrinkActor;
    actor->play = STGTRAIN_playActor;
    actor->pause = STGTRAIN_pauseActor;
    actor->setScale = STGTRAIN_setActorScale;
    actor->getResult = STGTRAIN_getActorResult;
    actor->set = set;
    actor->file = file;
    actor->layerId = layerId;
    actor->depth = depth;
    actor->end = STGTRAIN_endActor;
    return actor;
}
