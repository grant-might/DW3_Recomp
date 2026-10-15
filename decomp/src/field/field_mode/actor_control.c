/* An actor's moves, controls, methods, drawing and actions. One file: each
   of the jump tables of these functions starts right where the one
   before ends, 4 bytes past a multiple of 8, so a new object between
   them would move the next one's table. */

#include "field/field_mode.h"

/* Whether a flying actor runs into the map at an offset from it: a cell that
   isn't free, a wall higher than it or a ceiling lower than it (FLIGHT_WALL,
   FLIGHT_CEILING); then it steps back by offset */
s32 FIELDSTG_checkFlightProbe(Actor *actor, s32 x, s32 y, Vec2 offset) {
    s32 blocked = 0;
    Vec2 pos;
    u8 cell;

    pos.x = (actor->pos.x >> 8) + x;
    pos.y = (actor->pos.y >> 8) + y;
    cell = FIELDSTG_map.isTileFree(&pos);
    if (cell != 0) {
        cell = FIELDSTG_map.getCell(GAME.mapIndex, &pos);
    }
    if (actor->z != 0 && cell != FLIGHT_OPEN) {
        switch (cell) {
        case FLIGHT_WALL(2):
            if (actor->z < FLIGHT_LEVEL(2)) {
                cell = 0;
            }
            break;
        case FLIGHT_WALL(3):
            if (actor->z < FLIGHT_LEVEL(3)) {
                cell = 0;
            }
            break;
        case FLIGHT_WALL(4):
            if (actor->z < FLIGHT_LEVEL(4)) {
                cell = 0;
            }
            break;
        case FLIGHT_WALL(5):
            if (actor->z < FLIGHT_LEVEL(5)) {
                cell = 0;
            }
            break;
        case FLIGHT_WALL(6):
            if (actor->z < FLIGHT_LEVEL(6)) {
                cell = 0;
            }
            break;
        }
        switch (cell) {
        case FLIGHT_CEILING(6):
            if (actor->z > FLIGHT_LEVEL(6)) {
                cell = 0;
            }
            break;
        case FLIGHT_CEILING(5):
            if (actor->z > FLIGHT_LEVEL(5)) {
                cell = 0;
            }
            break;
        case FLIGHT_CEILING(4):
            if (actor->z > FLIGHT_LEVEL(4)) {
                cell = 0;
            }
            break;
        case FLIGHT_CEILING(3):
            if (actor->z > FLIGHT_LEVEL(3)) {
                cell = 0;
            }
            break;
        case FLIGHT_CEILING(2):
            if (actor->z > FLIGHT_LEVEL(2)) {
                cell = 0;
            }
            break;
        }
        if (cell == 0) {
            blocked = 1;
        }
    }
    if (cell == 0) {
        actor->pos.x -= offset.x;
        actor->pos.y -= offset.y;
    }
    return blocked;
}

/* Whether a flying actor runs into the map at any of the five probes ahead
   of it (FIELDSTG_checkFlightProbe) */
s32 FIELDSTG_checkFlightProbes(Actor *actor) {
    s32 blocked = 0;
    s32 i;
    u8 probe;
    u8 *sign;
    Vec2 offset;

    for (i = 0; i < 5; i++) {
        probe = FIELDSTG_probes[actor->dir][i];
        sign = FIELDSTG_probeSteps[probe];
        offset.x = (sign[0] & 1) * actor->speed / 2;
        if (sign[0] & 0x80) {
            offset.x = -offset.x;
        }
        offset.y = (sign[1] & 1) * actor->speed / 4;
        if (sign[1] & 0x80) {
            offset.y = -offset.y;
        }
        if (FIELDSTG_checkFlightProbe(actor, FIELDSTG_probePos[probe].x, FIELDSTG_probePos[probe].y, offset)) {
            blocked = 1;
        }
    }
    return blocked;
}

/* Walks or runs an actor in the direction of the pad's arrows (an index of
   FIELDSTG_padDirs), or stops it when none is held */
void FIELDSTG_moveByPad(Actor *actor, s32 pad) {
    if (pad != 0 && FIELDSTG_state.menuOpen == 0) {
        actor->dir = FIELDSTG_padDirs[pad];
        if (actor->walks != 0) {
            if (actor->substate != ACTOR_WALK) {
                actor->setSubstate(actor, ACTOR_WALK);
            }
        } else if (actor->substate != ACTOR_RUN) {
            actor->setSubstate(actor, ACTOR_RUN);
        }
    } else {
        if (actor->substate == ACTOR_WALK) {
            actor->setSubstate(actor, ACTOR_STAND);
        }
        if (actor->substate == ACTOR_RUN) {
            actor->setSubstate(actor, ACTOR_STOP);
        }
    }
}

/*
 * The first actor (task id 5) whose box holds the tile pos: halfWidth to
 * each side of it, half that above and below. The match depends on the
 * registry being held in a variable.
 */
Actor *FIELDSTG_findActorAt(Vec2 *pos) {
    TaskRegistry *registry = &TASK_REGISTRY;
    Actor *actor = registry->funcs.find(FIELD_TASK_ACTOR, -1, 1);

    while (actor != NULL) {
        s32 size = actor->halfWidth;

        if (pos->x >= actor->tile.x - size && actor->tile.x + size >= pos->x) {
            size >>= 1;
            if (pos->y >= actor->tile.y - size && actor->tile.y + size >= pos->y) {
                return actor;
            }
        }
        actor = registry->funcs.findNext();
    }
    return NULL;
}

/* Talks to the character standing at pos, or to the one it stands in for
   (FIELDSTG_standIns): it answers (ACTOR_TALK), or, for the objects 0x148,
   0x15F and 0x160, the player works it (ACTOR_USE). Returns 1 when the
   player acts */
s32 FIELDSTG_talkToActorAt(Actor *actor, Vec2 *pos) {
    Actor *target;
    s32 i;
    s32 result;

    target = FIELDSTG_findActorAt(pos);
    result = 0;
    if (target != NULL && target->state == TASK_RUN) {
        if (target->key1 != 0x180) {
            if (target->key1 != 0x181) {
                if (target->key1 != 0x182) {
                    for (i = 0; FIELDSTG_standIns[i].character != 0; i++) {
                        if (target->key1 == FIELDSTG_standIns[i].character) {
                            target = TASK_REGISTRY.funcs.find(FIELD_TASK_ACTOR, FIELDSTG_standIns[i].answerer, -1);
                            break;
                        }
                    }
                    switch (target->key1) {
                        case 0x148:
                        case 0x15F:
                        case 0x160:
                            if (target->substate != ACTOR_USED) {
                                target->setSubstate(target, ACTOR_USED);
                                result = 1;
                                target->talkPartner = actor;
                                actor->setSubstate(actor, ACTOR_USE);
                                actor->control = NULL;
                            }
                            break;
                        default:
                            target->setSubstate(target, ACTOR_TALK);
                            target->talkPartner = actor;
                            actor->setSubstate(actor, actor->flying != 0 ? ACTOR_FLOAT : ACTOR_STAND);
                            actor->control = NULL;
                            result = FIELDSTG_state.acting = 1;
                            break;
                    }
                }
            }
        }
    }
    return result;
}

/* Stops the sound of the flight, if it plays */
static inline void stopFlightSound(Actor *actor) {
    if (actor->voice != -1) {
        SOUND.keyOff(SOUND_SUB_MOVE, actor->voice);
        actor->voice = -1;
    }
}

/* Changes the flight's height speed (zSpeed) by 4 a frame, down to -0xC0
   while the player flies (ACTOR_FLY) and up to 0x80 while it floats */
static inline void accelerateFlight(Actor *actor) {
    if (actor->substate == ACTOR_FLY) {
        if (actor->zSpeed > -0xC0) {
            actor->zSpeed -= 4;
        } else {
            actor->zSpeed = -0xC0;
        }
    } else {
        if (actor->zSpeed < 0x80) {
            actor->zSpeed += 4;
        } else {
            actor->zSpeed = 0x80;
        }
    }
}

/*
 * Held TRIANGLE flies (ACTOR_FLY, with its sound), else the actor floats
 * and the sound stops
 */
static inline void flyOrFloat(Actor *actor, s32 held) {
    if (held & (1 << PAD_TRIANGLE)) {
        if (actor->substate != ACTOR_FLY) {
            actor->setSubstate(actor, ACTOR_FLY);
        }
        if (actor->voice == -1) {
            actor->voice = SOUND.playSound(SOUND_SUB_MOVE);
        }
    } else {
        if (actor->substate == ACTOR_FLY) {
            actor->setSubstate(actor, ACTOR_FLOAT);
        }
        stopFlightSound(actor);
    }
}

/* Held RIGHT or LEFT turns the actor one direction every 8 frames */
static inline void turnInFlight(Actor *actor, s32 held) {
    if (!(GFX.funcs.getFrameCount() & 7)) {
        if (held & (1 << PAD_RIGHT)) {
            actor->reloadImage = 1;
            actor->dir = (actor->dir + 1) & 7;
        } else if (held & (1 << PAD_LEFT)) {
            actor->reloadImage = 1;
            actor->dir = (actor->dir - 1) & 7;
        }
    }
}

/* The player's flight: it rises unless triangle is held (ACTOR_FLY),
   left and right turn, and cross, low enough, acts on what the player faces;
   the map's cells hold floors (2 to 6) and ceilings (18 to 22). The match
   depends on the button's shift and mask as two statements. */
void FIELDSTG_controlFlight(Actor *actor) {
    Vec2 facing;
    Vec2 pos;
    s32 held;
    s32 pressed;
    s32 cell;
    s32 stop;
    s32 height;

    held = PAD.getHeld(0);
    pressed = (u32)PAD.getPressed(0) >> PAD_CROSS;
    pressed &= 1;
    if (FIELDSTG_state.menuOpen != 0 || FIELDSTG_state.busy != 0 || FIELDSTG_state.battleStarting != 0) {
        return;
    }
    if (pressed && actor->z < FLIGHT_LEVEL(2)) {
        actor->getFacingTile(actor, &facing);
        if (FIELDSTG_talkToActorAt(actor, &facing)) {
            actor->zSpeed = 0;
            actor->speed = 0;
            stopFlightSound(actor);
            return;
        }
    }
    flyOrFloat(actor, held);
    turnInFlight(actor, held);
    accelerateFlight(actor);
    stop = 0;
    height = 0;
    pos.x = actor->pos.x >> 8;
    pos.y = actor->pos.y >> 8;
    cell = FIELDSTG_map.getCell(GAME.mapIndex, &pos);
    actor->z += actor->zSpeed;
    if (actor->zSpeed > 4) {
        switch ((u8)cell) {
        case FLIGHT_CEILING(6):
            if (actor->z > FLIGHT_LEVEL(6)) {
                stop = 1;
            }
            break;
        case FLIGHT_CEILING(5):
            if (actor->z > FLIGHT_LEVEL(5)) {
                stop = 1;
            }
            break;
        case FLIGHT_CEILING(4):
            if (actor->z > FLIGHT_LEVEL(4)) {
                stop = 1;
            }
            break;
        case FLIGHT_CEILING(3):
            if (actor->z > FLIGHT_LEVEL(3)) {
                stop = 1;
            }
            break;
        case FLIGHT_CEILING(2):
            if (actor->z > FLIGHT_LEVEL(2)) {
                stop = 1;
            }
            break;
        }
    } else {
        switch ((u8)cell) {
        case FLIGHT_WALL(2):
            if (actor->z < FLIGHT_LEVEL(2)) {
                stop = 1;
                height = FLIGHT_LEVEL(2) + FLIGHT_LANDING;
            }
            break;
        case FLIGHT_WALL(3):
            if (actor->z < FLIGHT_LEVEL(3)) {
                stop = 1;
                height = FLIGHT_LEVEL(3) + FLIGHT_LANDING;
            }
            break;
        case FLIGHT_WALL(4):
            if (actor->z < FLIGHT_LEVEL(4)) {
                stop = 1;
                height = FLIGHT_LEVEL(4) + FLIGHT_LANDING;
            }
            break;
        case FLIGHT_WALL(5):
            if (actor->z < FLIGHT_LEVEL(5)) {
                stop = 1;
                height = FLIGHT_LEVEL(5) + FLIGHT_LANDING;
            }
            break;
        case FLIGHT_WALL(6):
            if (actor->z < FLIGHT_LEVEL(6)) {
                stop = 1;
                height = FLIGHT_LEVEL(6) + FLIGHT_LANDING;
            }
            break;
        }
    }
    if (stop || actor->z > FLIGHT_Z_MAX || actor->z < FLIGHT_Z_MIN) {
        if (height != 0) {
            actor->z = height;
        }
        actor->zSpeed = 0;
    }
    if (actor->z >= FLIGHT_Z_MAX) {
        actor->z = FLIGHT_Z_MAX;
    }
    if (actor->z <= FLIGHT_Z_MIN) {
        actor->z = FLIGHT_Z_MIN;
    }
}

/*
 * The player's update while it walks (Actor.control) when no menu, event or
 * transition is running: the action button or a forced action (condition
 * 0x12) checks the tile it faces, for an object to act on or, failing that,
 * an event; otherwise the pad moves it. The match depends on the object's
 * case being an early exit, a do-while (0) with a break, after which the
 * event's check is the rest of the block.
 */
void FIELDSTG_controlPlayer(Actor *actor) {
    Vec2 pos;
    HiddenSpots *obj;
    s32 pad;
    s32 pressed;
    s32 forced;

    if (FIELDSTG_state.menuOpen == 0 && FIELDSTG_state.busy == 0 && FIELDSTG_state.battleStarting == 0) {
        pad = (PAD.getHeld(0) >> PAD_UP) & 0xF;
        pressed = (PAD.getPressed(0) & (1 << PAD_CROSS)) != 0;
        forced = FLAGS_00.checkCondition(FIELD_FLAG_TALK_AHEAD, 1);
        if (pressed || forced) {
            actor->getFacingTile(actor, &pos);
            do {
                if (FLAGS_00.checkCondition(FIELD_SEARCH_ITEM, 1) && !forced && (obj = FIELDSTG_findHiddenSpot(&pos, 1)) != NULL) {
                    FIELDSTG_state.acting = 1;
                    actor->setSubstate(actor, ACTOR_SEARCH);
                    obj->setState(obj, TASK_DONE);
                    break;
                }
                if (FIELDSTG_talkToActorAt(actor, &pos) && forced) {
                    FLAGS_00.applyAction(FIELD_FLAG_TALK_AHEAD, 0);
                }
            } while (0);
        } else {
            FIELDSTG_moveByPad(actor, pad);
        }
    }
}

/* The player's control on a wall: up and down climb, nothing held holds on */
void FIELDSTG_controlClimb(Actor *actor) {
    s32 held = PAD.getHeld(0);

    if (held & (1 << PAD_UP)) {
        if (actor->substate != ACTOR_CLIMB_UP) {
            actor->setSubstate(actor, ACTOR_CLIMB_UP);
        }
    } else if (held & (1 << PAD_DOWN)) {
        if (actor->substate != ACTOR_CLIMB_DOWN) {
            actor->setSubstate(actor, ACTOR_CLIMB_DOWN);
        }
    } else if (actor->substate != ACTOR_CLIMB) {
        actor->setSubstate(actor, ACTOR_CLIMB);
    }
}

/* A partner's control: records the leader's steps in the trail while it
   moves, and follows them */
void FIELDSTG_followLeader(Actor *actor) {
    Trail *trail;
    Actor *leader;

    if (actor->trail->leader == NULL) {
        actor->trail->leader = TASK_REGISTRY.funcs.find(FIELD_TASK_ACTOR, -1, 0);
    }
    leader = actor->trail->leader;
    if (leader != NULL) {
        trail = actor->trail;
        switch (leader->substate) {
            case ACTOR_WALK:
            case ACTOR_RUN:
            case ACTOR_WALK_OUT:
            case ACTOR_SLIDE:
            case ACTOR_STOP_SLIDE:
                trail->steps[trail->head].x = leader->pos.x;
                trail->steps[trail->head].y = leader->pos.y;
                trail->steps[trail->head].dir = leader->dir;
                trail->head = (trail->head + 1) & (TRAIL_STEPS - 1);
                actor->pos.x = trail->steps[trail->tail].x;
                actor->pos.y = trail->steps[trail->tail].y;
                actor->dir = trail->steps[trail->tail].dir;
                trail->tail = (trail->tail + 1) & (TRAIL_STEPS - 1);
                break;
        }
        switch (leader->substate) {
            case ACTOR_WALK:
            case ACTOR_RUN:
            case ACTOR_WALK_OUT:
                if (actor->substate != ACTOR_RUN) {
                    actor->setSubstate(actor, ACTOR_RUN);
                }
                break;
            case ACTOR_SLIDE:
                if (actor->substate != ACTOR_STAND) {
                    actor->setSubstate(actor, ACTOR_STAND);
                }
                break;
            default:
                if (actor->substate == ACTOR_RUN) {
                    actor->setSubstate(actor, ACTOR_STOP);
                }
                break;
        }
        actor->depth = leader->depth;
    }
}

/* A follower's update while its trail runs out: it finds the leader
   (FIELD_TASK_ACTOR) if it has none, then each frame pushes two empty
   steps at the trail's head and walks two steps from its tail. At x 0 it
   clears the trail and drops this update. The match depends on the leader
   being read into a variable before `trail` is set, which makes `trail` a
   copy of the pointer the leader was loaded through. */
void FIELDSTG_drainTrail(Actor *actor) {
    Trail *trail;
    Actor *leader;
    s32 i;

    if (actor->trail->leader == NULL) {
        actor->trail->leader = TASK_REGISTRY.funcs.find(FIELD_TASK_ACTOR, -1, 0);
    }
    leader = actor->trail->leader;
    trail = actor->trail;
    if (leader != NULL) {
        for (i = 0; i < 2; i++) {
            trail->steps[trail->head].x = 0;
            trail->steps[trail->head].y = 0;
            trail->steps[trail->head].dir = 0;
            trail->head = (trail->head + 1) & (TRAIL_STEPS - 1);
            actor->pos.x = trail->steps[trail->tail].x;
            actor->pos.y = trail->steps[trail->tail].y;
            actor->dir = trail->steps[trail->tail].dir;
            trail->tail = (trail->tail + 1) & (TRAIL_STEPS - 1);
        }
        if (actor->substate != ACTOR_RUN) {
            actor->setSubstate(actor, ACTOR_RUN);
        }
        if (actor->pos.x == 0) {
            for (i = 0; i < TRAIL_STEPS; i++) {
                trail->steps[i].dir = 0;
                trail->steps[i].x = 0;
                trail->steps[i].y = 0;
            }
            actor->control = NULL;
        }
    }
}

/* Walks an actor to its goal tile (FIELDSTG_setActorGoal) as the pad would,
   then faces it the goal's way */
void FIELDSTG_walkToGoal(Actor *actor) {
    s32 x;
    s32 y;
    s32 tx;
    s32 ty;
    s32 pad;

    if (actor->walking != 0) {
        x = actor->pos.x >> 8;
        y = actor->pos.y >> 8;
        tx = actor->goalX;
        ty = actor->goalY;
        if (x >> 1 != tx >> 1 || y >> 1 != ty >> 1) {
            pad = 0;
            if (x < tx) {
                pad = 1 << PAD_RIGHT;
            } else if (x > tx) {
                pad = 1 << PAD_LEFT;
            }
            if (y < ty) {
                pad |= 1 << PAD_DOWN;
            } else if (y > ty) {
                pad |= 1 << PAD_UP;
            }
            actor->walks = 1;
            FIELDSTG_moveByPad(actor, pad >> 4);
        } else {
            actor->pos.x = actor->goalX << 8;
            actor->walking = 0;
            actor->pos.y = actor->goalY << 8;
            actor->dir = actor->goalDir;
            actor->setSubstate(actor, ACTOR_STAND);
        }
    }
}

/* Sets the tile an actor walks to and the direction it ends facing
   (FIELDSTG_walkToGoal) */
void FIELDSTG_setActorGoal(Actor *actor, s32 x, s32 y, s32 dir) {
    actor->walking = 1;
    actor->goalX = x;
    actor->goalY = y;
    actor->goalDir = dir;
}

/* Whether an actor is still walking to its goal */
s32 FIELDSTG_isActorWalking(Actor *actor) {
    return actor->walking;
}

/* Starts an actor walking to its goal */
void FIELDSTG_startActorWalk(Actor *actor) {
    actor->control = FIELDSTG_walkToGoal;
}

/* Gives an actor its kind's control back: the pad for the player, none for
   kind 1, the trail for the partners */
void FIELDSTG_resetActorControl(Actor *actor) {
    switch (actor->key2) {
    case 0:
        actor->control = FIELDSTG_controlPlayer;
        actor->walks = 0;
        break;
    case 1:
        actor->control = NULL;
        break;
    case 2:
    case 4:
    case 8:
        actor->control = FIELDSTG_followLeader;
        break;
    }
}

/* Walks an actor out through an exit, in a direction (ACTOR_WALK_OUT) */
void FIELDSTG_walkActorInDir(Actor *actor, s32 dir) {
    actor->control = NULL;
    actor->setSubstate(actor, ACTOR_WALK_OUT);
    actor->dir = dir;
}

/* Starts an actor sliding in a direction (SLOT_SLIDE) */
void FIELDSTG_startActorSlide(Actor *actor, s32 dir) {
    if (actor->substate != ACTOR_SLIDE) {
        actor->control = NULL;
        actor->setSubstate(actor, ACTOR_SLIDE);
        actor->dir = dir;
    }
}

/* Stops a sliding actor (SLOT_STOP_SLIDE) */
void FIELDSTG_stopActorSlide(Actor *actor) {
    if (actor->substate == ACTOR_SLIDE) {
        actor->setSubstate(actor, ACTOR_STOP_SLIDE);
    }
}

/* The climbs (SLOT_CLIMB_UP and SLOT_CLIMB_DOWN) and the drop (SLOT_DROP):
   the actor goes onto the wall at its foot (ACTOR_GET_ON_WALL) or its top
   (ACTOR_GET_OVER_EDGE), climbs it with the pad (FIELDSTG_controlClimb) and
   leaves it at the top or the foot; a drop (ACTOR_DROP) falls from height. */
void FIELDSTG_startClimbUp(Actor *actor, s32 dir, s32 x, s32 y, s32 height) {
    actor->control = NULL;
    actor->setSubstate(actor, ACTOR_GET_ON_WALL);
    actor->dir = dir;
    actor->pos.x = x << 8;
    actor->pos.y = y << 8;
    actor->climbHeight = 0;
    actor->wallHeight = height << 8;
    if (dir != 5) {
        actor->climbSide = 1;
    } else {
        actor->climbSide = 0;
    }
    FIELDSTG_haltPartners();
    FIELDSTG_state.acting = 1;
}

/* Starts the climb down a wall of height from its top, at (x, y) */
void FIELDSTG_startClimbDown(Actor *actor, s32 dir, s32 x, s32 y, s32 height) {
    actor->control = NULL;
    actor->setSubstate(actor, ACTOR_GET_OVER_EDGE);
    actor->dir = dir;
    actor->pos.x = x << 8;
    actor->pos.y = y << 8;
    actor->climbHeight = actor->wallHeight = height << 8;
    if (dir != 5) {
        actor->climbSide = 1;
    } else {
        actor->climbSide = 0;
    }
    actor->hasShadow = 0;
    FIELDSTG_haltPartners();
    FIELDSTG_state.acting = 1;
}

/* Starts a drop from a ledge of height */
void FIELDSTG_startDrop(Actor *actor, s32 dir, Vec2 pos, s32 height) {
    s32 value;

    actor->control = NULL;
    actor->setSubstate(actor, ACTOR_DROP);
    actor->dir = dir;
    if (dir != 7) {
        actor->climbSide = 0;
    } else {
        actor->climbSide = 1;
    }
    value = height << 8;
    actor->wallHeight = value;
    actor->hasShadow = 0;
    actor->climbHeight = value;
    FIELDSTG_haltPartners();
    FIELDSTG_state.acting = 1;
}

/* Starts the gauge game at an offset from an actor, turning the partners its
   way */
void FIELDSTG_startActorGauge(Actor *actor, s32 dir, Vec2 offset) {
    void **children;
    Actor *other;
    Vec2 pos;
    s32 i;

    FIELDSTG_state.busy = 1;
    actor->control = NULL;
    actor->setSubstate(actor, ACTOR_GAUGE);
    actor->dir = dir;
    for (i = 0; i < 3; i++) {
        other = TASK_REGISTRY.funcs.find(FIELD_TASK_ACTOR, -1, FIELDSTG_turnedPartners[i]);
        if (other != NULL) {
            other->dir = dir;
        }
    }
    children = actor->children;
    pos.x = actor->tile.x + offset.x;
    pos.y = actor->tile.y + offset.y;
    children[2] = FIELDSTG_createGauge(pos);
}

/* Warps the player (SLOT_WARP0 and SLOT_WARP1): the field task plays the
   warp's effect and cutscene of kind, then leaves for its mode
   (FIELDSTG_startWarp) */
void FIELDSTG_warpActor(Actor *actor, SlotDest *dest, s32 kind) {
    actor->control = NULL;
    actor->setSubstate(actor, ACTOR_STAND);
    actor->dir = 0;
    FIELDSTG_state.busy = 1;
    FIELDSTG_startWarp(kind, &actor->tile, dest);
}

/* Sends the player flying to the tile at dest from the nearest launcher
   (FIELDSTG_createLaunch) */
void FIELDSTG_launchActor(Actor *actor, SlotDest *dest) {
    void **children;

    actor->control = FIELDSTG_walkToGoal;
    actor->setSubstate(actor, ACTOR_STAND);
    actor->dir = 0;
    FIELDSTG_state.busy = 1;
    children = actor->children;
    children[2] = FIELDSTG_createLaunch(actor, dest);
}

/* Starts an actor's animation set from its first frame */
void FIELDSTG_setActorAnim(Actor *actor, s32 set) {
    actor->animSet = set;
    actor->animPos = 0;
    actor->animTime = 0;
    actor->animDone = 0;
}

/* Puts an actor in an event's pose: an animation set and a direction
   (ACTOR_POSED) */
void FIELDSTG_setActorPose(Actor *actor, s32 set, s32 dir) {
    actor->walking = 0;
    actor->setSubstate(actor, ACTOR_POSED);
    actor->dir = dir;
    FIELDSTG_setActorAnim(actor, set);
}

/* Whether an actor's animation has played to its end */
s32 FIELDSTG_isActorAnimDone(Actor *actor) {
    return actor->animDone;
}

/* The layer's sorted callback that draws an actor: its frame from its image,
 * mirrored for the directions 5 and up, and, when it has one, its shadow, a
 * sprite with its own texture page. The match depends on the shadow's y offset
 * cast to s16, and on the untyped parameters copied to typed locals. */
void FIELDSTG_drawActor(void *data, void *drawLayer) {
    Actor *actor = data;
    Layer *layer = drawLayer;
    ActorImage *image = actor->image;
    Vec2 pos;
    Vec2 scroll;
    u_long *ot;
    PrimPtr prim;
    FieldImage *shadow;
    s32 x;

    if (actor->state == TASK_RUN) {
        ot = (u_long *)layer->getOtEntry(layer, actor->depth);
        pos = actor->tile;
        layer->getScroll(layer, &scroll);
        pos.x -= scroll.x;
        pos.y -= scroll.y;
        prim.any = GFX.funcs.getPrim();
        pos.y -= actor->z >> 8;
        setPolyFT4(prim.ft4);
        setRGB0(prim.ft4, 0x80, 0x80, 0x80);
        x = actor->frame[2];
        if (actor->dir >= 5) {
            x = -(actor->frameWidth + x);
        }
        prim.ft4->x0 = pos.x + x;
        prim.ft4->x1 = actor->frameWidth + (pos.x + x);
        prim.ft4->x2 = pos.x + x;
        prim.ft4->x3 = actor->frameWidth + (pos.x + x);
        prim.ft4->y0 = actor->frame[3] + pos.y;
        prim.ft4->y1 = actor->frame[3] + pos.y;
        prim.ft4->y2 = actor->frameHeight + (actor->frame[3] + pos.y);
        prim.ft4->y3 = actor->frameHeight + (actor->frame[3] + pos.y);
        if (actor->dir < 5) {
            prim.ft4->u0 = image->u;
            prim.ft4->u1 = actor->frameWidth + image->u;
            prim.ft4->u2 = image->u;
            prim.ft4->u3 = actor->frameWidth + image->u;
        } else {
            prim.ft4->x1--;
            prim.ft4->x3--;
            prim.ft4->u0 = actor->frameWidth + image->u - 1;
            prim.ft4->u1 = image->u;
            prim.ft4->u2 = actor->frameWidth + image->u - 1;
            prim.ft4->u3 = image->u;
        }
        prim.ft4->v0 = image->v;
        prim.ft4->v1 = image->v;
        prim.ft4->v2 = actor->frameHeight + image->v;
        prim.ft4->v3 = actor->frameHeight + image->v;
        prim.ft4->clut = getClut(image->clutX, image->clutY);
        prim.ft4->tpage = getTPage(0, 0, image->x, image->y);
        addPrim(ot, prim.any);
        pos.y += actor->z >> 8;
        prim.ft4++;
        if (actor->hasShadow != 0) {
            ot = (u_long *)layer->getOtEntry(layer, actor->depth + 1);
            shadow = actor->fieldImage;
            setSprt(prim.sprt);
            setRGB0(prim.sprt, 0x80, 0x80, 0x80);
            prim.sprt->x0 = pos.x - 16;
            prim.sprt->y0 = pos.y + (s16)((actor->climbHeight >> 8) - 8);
            prim.sprt->u0 = shadow->shadow.u;
            prim.sprt->v0 = shadow->shadow.v;
            prim.sprt->w = 0x20;
            prim.sprt->h = 0x10;
            prim.sprt->clut = getClut(shadow->shadow.clutX, shadow->shadow.clutY);
            addPrim(ot, prim.any);
            prim.sprt++;
            SetDrawTPage(prim.tpage, 0, 1, GetTPage(0, 0, shadow->shadow.x, shadow->shadow.y));
            addPrim(ot, prim.any);
            prim.tpage++;
        }
        GFX.funcs.setPrim(prim.any);
    }
}

/* Turns an actor to a direction */
void FIELDSTG_setActorDir(Actor *actor, s32 dir) {
    actor->dir = dir;
}

#if VERSION_US || VERSION_JP
#define SET_FILE_0 0x01790070
#define SET_FILE_8 0x03BB001E
#define SET_FILE_16 0x03B9000B
#define SET_FILE_1A 0x03BC0017
#define SET_FILE_OTHER 0x03BA0064
#elif VERSION_EU
#define SET_FILE_0 0x01870070
#define SET_FILE_8 0x03CB001E
#define SET_FILE_16 0x03C9000B
#define SET_FILE_1A 0x03CC0017
#define SET_FILE_OTHER 0x03CA0064
#endif

/*
 * Plays an actor's animation: when its set (animSet) changes, it loads the
 * set's animations for the five directions (character 2 picks the file by
 * the set first), then steps the frames of the current direction's and
 * loads a new frame's image into VRAM. The match depends on two early
 * exits written as a do-while (0) with breaks, the pick of the file and
 * the step of the frames: loop.c moves the end of the animation (-1) out
 * of the second one, after the direction's barrier, and reorg fills the
 * first one's < 0x16 branch from its fallthrough, as only a branch out of
 * a loop is. It also depends on reloadImage being read twice, on the width cast
 * to s16 and on reloadImage being cleared last.
 */
void FIELDSTG_animateActor(Actor *actor) {
    TimLoader loader;
    s16 *entry;
    s32 *frames;
    s32 set;
    s32 file;
    s32 dir;
    s32 i;
    ActorImage *image;

    set = actor->animSet;
    file = actor->animFile & 0xFFFF0000;
    if (actor->loadedSet != set) {
        if (actor->key1 == 2) {
            do {
                if (set < 8) {
                    actor->animFile = SET_FILE_0;
                    break;
                }
                if (set == 8) {
                    actor->animFile = SET_FILE_8;
                    break;
                }
                if (set < 0x16) {
                    actor->animFile = SET_FILE_OTHER;
                    break;
                }
                if (set < 0x1A) {
                    actor->animFile = SET_FILE_16;
                    break;
                }
                if (set < 0x25) {
                    actor->animFile = SET_FILE_1A;
                    break;
                }
                actor->animFile = SET_FILE_OTHER;
            } while (0);
            file = actor->animFile & 0xFFFF0000;
        }
        for (entry = FILE_CACHE.getEntry(actor->animFile); entry[0] != 0; entry += 6) {
            if (entry[0] == set) {
                break;
            }
        }
        if (entry[0] == 0) {
            entry = FILE_CACHE.getEntry(actor->animFile);
        }
        for (i = 0; i < 5; i++) {
            actor->setAnims[i] = entry[i + 1] | file;
        }
        actor->loadedSet = set;
        actor->animDone = 0;
    }
    if (actor->dir < 5) {
        dir = actor->dir;
    } else {
        dir = 8 - actor->dir;
    }
    frames = FILE_CACHE.getEntry(actor->setAnims[dir]);
    do {
        if (actor->animTime > 0) {
            break;
        }
        if (frames[actor->animPos] == -1) {
            actor->animTime = 0x7FFFFF;
            actor->animDone = 1;
            break;
        }
        if (frames[actor->animPos] == 0) {
            actor->animPos = 0;
        }
        actor->animTime = frames[actor->animPos++];
        actor->frame[0] = frames[actor->animPos++] | file;
        actor->frame[2] = frames[actor->animPos++];
        actor->frame[3] = frames[actor->animPos++];
    } while (0);
    actor->animTime -= GFX.funcs.getFrameTime();
    if (actor->reloadImage) {
        actor->animTime = frames[actor->animPos - 4];
        actor->frame[0] = frames[actor->animPos - 3] | file;
        actor->frame[2] = frames[actor->animPos - 2];
        actor->frame[3] = frames[actor->animPos - 1];
    }
    if (actor->reloadImage || actor->frame[1] != actor->frame[0]) {
        image = actor->image;
        initTimLoader(&loader);
        loader.setImagePos(image->imageX, image->imageY);
        loader.setClutPos(image->clutX, image->clutY);
        loader.load(FILE_CACHE.getEntry(actor->frame[0]));
        actor->frame[1] = actor->frame[0];
        actor->frameWidth = (s16)loader.w * 4;
        actor->frameHeight = loader.h;
        actor->reloadImage = 0;
    }
}

/* Gives the player its pad control back, off any wall */
void FIELDSTG_restorePlayerControl(Actor *actor) {
    actor->control = FIELDSTG_controlPlayer;
    actor->climbHeight = 0;
}

/* The player's footsteps, every 8 frames when it moves (every 32 for the
   characters 0x146 and 0x147) or 32 when not; with canBattle each step can
   start a battle (FIELDSTG_checkBattle) */
void FIELDSTG_playStepSounds(Actor *actor, s32 moving, s32 canBattle) {
    if (actor->key2 == 0) {
        if (moving != 0) {
            if ((actor->counter & 7) == 0) {
                if (actor->key1 != 0x146) {
                    if (actor->key1 != 0x147) {
                        SOUND.playSound(SOUND_PLAYER00);
                    }
                } else if ((actor->counter & 0x1F) == 0) {
                    SOUND.playSound(SOUND_DIG_MOVE);
                }
                if (canBattle != 0) {
                    FIELDSTG_checkBattle();
                }
            }
        } else if ((actor->counter & 0x1F) == 0) {
            SOUND.playSound(SOUND_PLAYER00);
        }
        actor->counter++;
    }
}

/* The player's climbing sound, every 16 frames */
void FIELDSTG_playClimbSounds(Actor *actor) {
    if (actor->key2 == 0) {
        if ((actor->counter & 0xF) == 0) {
            SOUND.playSound(SOUND_PLAYER01);
        }
        actor->counter++;
    }
}

/* The first frame of an action: the actor gets its shadow and the action's
   animation */
static inline void beginActorAnim(Actor *actor, s32 anim) {
    switch (actor->step) {
    case 0:
    default:
        actor->hasShadow = 1;
        FIELDSTG_setActorAnim(actor, anim);
        actor->nextStep(actor);
    case 1:
        break;
    }
}

/* Puts a character back to standing, under the player's control again */
static inline void standAgain(Actor *actor) {
    actor->setSubstate(actor, ACTOR_STAND);
    FIELDSTG_setActorAnim(actor, ACTOR_ANIM_STAND);
    FIELDSTG_restorePlayerControl(actor);
}

/* Gives the player that talked back its control, walking or flying */
static inline void giveControlBack(Actor *player) {
    if (player->flying == 0) {
        FIELDSTG_restorePlayerControl(player);
    } else {
        player->control = FIELDSTG_controlFlight;
    }
}

/* Sets the flag of the characters 0x148, 0x15F and 0x160 once they are used */
static inline void setUsedFlag(s32 character) {
    switch (character) {
    case 0x148:
        FLAGS_00.applyAction(8, 1);
        break;
    case 0x15F:
        FLAGS_00.applyAction(9, 1);
        break;
    case 0x160:
        FLAGS_00.applyAction(0xA, 1);
        break;
    }
}

/* ACTOR_STOP: the stop after a run, then standing */
static inline void runStop(Actor *actor) {
    switch (actor->step) {
    case 0:
    default:
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_STOP);
        actor->nextStep(actor);
    case 1:
        break;
    }
    if (actor->animDone != 0) {
        actor->setSubstate(actor, ACTOR_STAND);
    }
}

/* ACTOR_SLIDE: the slide on ice, with its sound */
static inline void runSlide(Actor *actor) {
    Vec2 move;

    switch (actor->step) {
    case 0:
    default:
        FIELDSTG_state.acting = 1;
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_STAND);
        FIELDSTG_slideVoice = SOUND.playSound(SOUND_TRAP_ICE);
        actor->nextStep(actor);
    case 1:
        break;
    }
    FIELDSTG_map.getWalkStep(&actor->tile, actor->speed, actor->dir, &move);
    actor->pos.x += move.x;
    actor->pos.y += move.y;
}

/* ACTOR_STOP_SLIDE: the end of a slide, half a second before the player's
   control comes back */
static inline void runStopSlide(Actor *actor) {
    if (actor->step == 0) {
        SOUND.keyOff(SOUND_TRAP_ICE, FIELDSTG_slideVoice);
    }
    actor->step += GFX.funcs.getFrameTime();
    if (actor->step >= 0x1E) {
        FIELDSTG_state.acting = 0;
        actor->setSubstate(actor, ACTOR_STAND);
        FIELDSTG_restorePlayerControl(actor);
    }
}

/* ACTOR_CLIMB_UP: up the wall, to its top */
static inline void runClimbUp(Actor *actor) {
    switch (actor->step) {
    case 0:
    default:
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_CLIMB_UP);
        actor->nextStep(actor);
    case 1:
        break;
    }
    actor->climbHeight += 0x100;
    if (actor->climbHeight >= actor->wallHeight) {
        actor->climbHeight = actor->wallHeight;
        actor->setSubstate(actor, ACTOR_CLIMB_OFF_TOP);
        actor->control = NULL;
    }
    FIELDSTG_playClimbSounds(actor);
}

/* ACTOR_CLIMB_DOWN: down the wall, to its foot */
static inline void runClimbDown(Actor *actor) {
    switch (actor->step) {
    case 0:
    default:
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_CLIMB_DOWN);
        actor->nextStep(actor);
    case 1:
        break;
    }
    actor->climbHeight -= 0x100;
    if (actor->climbHeight <= 0) {
        actor->climbHeight = 0;
        actor->setSubstate(actor, ACTOR_CLIMB_OFF_BOTTOM);
        actor->control = NULL;
    }
    FIELDSTG_playClimbSounds(actor);
}

/* ACTOR_GET_ON_WALL: from the ground onto the wall, to climb it */
static inline void runGetOnWall(Actor *actor) {
    switch (actor->step) {
    case 0:
    default:
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_GET_ON_WALL);
        actor->nextStep(actor);
    case 1:
        break;
    }
    if (actor->animDone != 0) {
        actor->setSubstate(actor, ACTOR_CLIMB);
        actor->control = FIELDSTG_controlClimb;
    }
}

/* ACTOR_CLIMB_OFF_BOTTOM: from the wall's foot onto the ground */
static inline void runClimbOffBottom(Actor *actor) {
    switch (actor->step) {
    case 0:
    default:
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_CLIMB_OFF_BOTTOM);
        actor->nextStep(actor);
    case 1:
        break;
    }
    if (actor->animDone != 0) {
        actor->setSubstate(actor, ACTOR_STAND);
        FIELDSTG_restorePlayerControl(actor);
        FIELDSTG_resumePartners();
        actor->dir = 0;
        FIELDSTG_state.acting = 0;
    }
}

/* ACTOR_CLIMB_OFF_TOP: from the wall's top onto the ground above */
static inline void runClimbOffTop(Actor *actor) {
    switch (actor->step) {
    case 0:
    default:
        actor->hasShadow = 0;
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_CLIMB_OFF_TOP);
        actor->nextStep(actor);
    case 1:
        break;
    }
    if (actor->animDone != 0) {
        actor->setSubstate(actor, ACTOR_STAND);
        actor->hasShadow = 1;
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_STAND);
        FIELDSTG_resumePartners();
        FIELDSTG_restorePlayerControl(actor);
        actor->pos.x += actor->climbSide != 0 ? -0x1000 : 0x1000;
        actor->pos.y -= 0x1800 + actor->wallHeight;
        FIELDSTG_followWithCamera(0, 2);
        FIELDSTG_state.acting = 0;
    }
}

/* ACTOR_DROP: the drop off a wall's edge, the fall and the landing */
static inline void runDrop(Actor *actor) {
    switch (actor->step) {
    case 0:
    default:
        actor->hasShadow = 0;
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_DROP_OFF);
        actor->pos.y += actor->wallHeight;
        actor->pos.x += actor->climbSide != 0 ? 0x1000 : -0x1000;
        actor->nextStep(actor);
        /* fallthrough */
    case 1:
        if (actor->animDone == 0) {
            break;
        }
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_FALL);
        actor->nextStep(actor);
        /* fallthrough */
    case 2:
        actor->hasShadow = 1;
        actor->climbHeight -= 0x300;
        if (actor->animSet == ACTOR_ANIM_FALL && actor->wallHeight - actor->climbHeight > 0x1800) {
            FIELDSTG_setActorAnim(actor, ACTOR_ANIM_FALL_FAR);
        }
        if (actor->climbHeight <= 0) {
            actor->climbHeight = 0;
            FIELDSTG_setActorAnim(actor, ACTOR_ANIM_LAND);
            SOUND.playSound(SOUND_PLAYER02);
            actor->nextStep(actor);
        }
        break;
    case 3:
        if (actor->animDone != 0) {
            standAgain(actor);
            FIELDSTG_resumePartners();
            FIELDSTG_state.acting = 0;
        }
        break;
    }
}

/* ACTOR_GAUGE: the gauge game's poses, from its start to its result's balloon */
static inline void runGauge(Actor *actor, ActorChildren *children) {
    switch (actor->step) {
    case 0:
    default:
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_GAUGE_START);
        actor->nextStep(actor);
        /* fallthrough */
    case 1:
        if (actor->animDone == 0) {
            break;
        }
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_GAUGE_PLAY);
        SOUND.playSound(SOUND_PLAYER09);
        actor->nextStep(actor);
        /* fallthrough */
    case 2:
        if (actor->animDone == 0) {
            break;
        }
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_GAUGE_WAIT);
        SOUND.playSound(SOUND_PLAYER10);
        actor->nextStep(actor);
        /* fallthrough */
    case 3:
        if (children->action != NULL) {
            break;
        }
        children->balloon = FIELDSTG_createBalloon(0, 1, 6);
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_GAUGE_RESULT);
        actor->nextStep(actor);
        /* fallthrough */
    case 4:
        actor->counter += GFX.funcs.getFrameTime();
        if (actor->counter < 0x3C) {
            break;
        }
        children->balloon->setState(children->balloon, TASK_DONE);
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_GAUGE_END);
        actor->nextStep(actor);
        /* fallthrough */
    case 5:
        if (actor->animDone != 0) {
            standAgain(actor);
            FIELDSTG_state.busy = 0;
        }
        break;
    }
}

/* ACTOR_SEARCH: the player searches a hidden spot */
static inline void runSearch(Actor *actor) {
    switch (actor->step) {
    case 0:
    default:
        actor->control = NULL;
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_SEARCH);
        actor->nextStep(actor);
    case 1:
        break;
    }
    if (actor->animDone != 0) {
        standAgain(actor);
        FIELDSTG_state.acting = 0;
    }
}

/* ACTOR_TALK: the character's first talk whose conditions hold, then the
   player's control back and the talk's flag actions */
static inline void runTalk(Actor *actor, ActorChildren *children) {
    FieldTalk *talk;
    s32 isTreasure;

    isTreasure = 0;
    if (actor->key1 == 0x21 || (actor->key1 >= 0x4D && actor->key1 < 0x58) ||
        (actor->key1 >= 0x154 && actor->key1 < 0x159) || actor->key1 == 0x15B) {
        isTreasure = 1;
    }
    switch (actor->step) {
    case 0:
    default:
        talk = actor->entry->talks;
        while (1) {
            if (talk->conditions == NULL) {
                break;
            }
            if (FLAGS_00.checkConditions(talk->conditions) == 1) {
                break;
            }
            talk++;
        }
        actor->talkActions = talk->actions;
        if (!isTreasure && actor->keepsDir == 0) {
            actor->dir = (actor->talkPartner->dir + 4) & 7;
        }
        if (actor->animFile != 0 && !isTreasure) {
            children->speech = FIELDSTG_createTalk(actor, talk->entry);
        } else {
            children->speech = FIELDSTG_createTalk(actor->talkPartner, talk->entry);
        }
        if (isTreasure) {
            FIELDSTG_setActorAnim(actor, ACTOR_ANIM_OPEN);
            SOUND.playSound(SOUND_TRESUREB);
        }
        actor->nextStep(actor);
        break;
    case 1:
        if (children->speech == NULL) {
            if (!isTreasure) {
                actor->setSubstate(actor, ACTOR_STAND);
            } else {
                actor->setState(actor, TASK_KILL);
            }
            giveControlBack(actor->talkPartner);
            if (actor->talkActions != NULL) {
                FLAGS_00.applyActions(actor->talkActions);
            }
            FIELDSTG_state.acting = 0;
        }
        break;
    }
}

/* ACTOR_USE: the player works one of the objects 0x148, 0x15F and 0x160 */
static inline void runUse(Actor *actor) {
    switch (actor->step) {
    case 0:
    default:
#if VERSION_EU || VERSION_JP
        FIELDSTG_state.acting = 1;
#endif
        actor->control = NULL;
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_USE);
        actor->nextStep(actor);
    case 1:
        break;
    }
    if (actor->animDone != 0) {
        standAgain(actor);
#if VERSION_EU || VERSION_JP
        FIELDSTG_state.acting = 0;
#endif
    }
}

/* ACTOR_USED: the object the player worked, which plays its animation, sets its
   flag and goes away */
static inline void runUsed(Actor *actor) {
    switch (actor->step) {
    case 0:
    default:
        if (actor->counter < 0x14) {
            actor->counter += GFX.funcs.getFrameTime();
            break;
        }
        FIELDSTG_setActorAnim(actor, ACTOR_ANIM_USED);
        SOUND.playSound(SOUND_COMEX113);
#if VERSION_EU || VERSION_JP
        setUsedFlag(actor->key1);
#endif
        actor->nextStep(actor);
        /* fallthrough */
    case 1:
        if (actor->animDone != 0) {
#if VERSION_US
            setUsedFlag(actor->key1);
#endif
            actor->setState(actor, TASK_KILL);
        }
        break;
    }
}

/*
 * Runs an actor's action, its substate (ACTOR_STAND...): the walks, the
 * climbs and the drop from ACTOR_GET_OVER_EDGE to ACTOR_DROP (which shift it
 * by a tile, left or right by climbSide), the talk of ACTOR_TALK (the first
 * of the character's talks whose conditions hold) and the others up to
 * ACTOR_STOP_SLIDE. A treasure (characters 0x21, 0x4D to 0x57, 0x154 to 0x158
 * and 0x15B, whose talks give an item) doesn't turn to its talker: it opens
 * with TRESUREB, and goes away when the talk ends. The match depends on the talks' loop testing both of its
 * ends with a break at its top, and on the moves of a tile adding a choice of
 * two steps.
 */
void FIELDSTG_runActorAction(Actor *actor, ActorChildren *children) {
    Vec2 move2;
    Vec2 move3;
    Vec2 move4;
    Vec2 move5;

    switch (actor->substate) {
    case ACTOR_STAND:
        beginActorAnim(actor, ACTOR_ANIM_STAND);
        break;
    case ACTOR_WALK:
        beginActorAnim(actor, ACTOR_ANIM_WALK);
        if (!(actor->key2 & 0xE)) {
            FIELDSTG_map.getWalkStep(&actor->tile, actor->speed >> 2, actor->dir, &move2);
            actor->pos.x += move2.x;
            actor->pos.y += move2.y;
        }
        FIELDSTG_playStepSounds(actor, 0, 0);
        break;
    case ACTOR_RUN:
        beginActorAnim(actor, ACTOR_ANIM_RUN);
        if (!(actor->key2 & 0xE)) {
            FIELDSTG_map.getWalkStep(&actor->tile, actor->speed, actor->dir, &move3);
            actor->pos.x += move3.x;
            actor->pos.y += move3.y;
        }
        FIELDSTG_playStepSounds(actor, 1, 1);
        if (GAME.funcs.getMode() != FIELD_MODE_WSTAG415 && actor->key2 == 0) {
            FIELDSTG_checkFlightProbes(actor);
        }
        break;
    case ACTOR_FLOAT:
        beginActorAnim(actor, ACTOR_ANIM_STAND);
        if (actor->speed != 0) {
            actor->speed -= 8;
            if (actor->speed < 0) {
                actor->speed = 0;
            }
            FIELDSTG_map.getFlyStep(&actor->tile, actor->speed, actor->dir, &move4);
            actor->pos.x += move4.x;
            actor->pos.y += move4.y;
            FIELDSTG_checkFlightProbes(actor);
        }
        break;
    case ACTOR_FLY:
        switch (actor->step) {
        case 0:
        default:
            actor->hasShadow = 1;
            FIELDSTG_setActorAnim(actor, ACTOR_ANIM_WALK);
            actor->speed = 0;
            actor->nextStep(actor);
        case 1:
            break;
        }
        actor->speed += 8;
#if PAL_VIDEO
        if (NTSC_MODE != 0) {
            if (actor->speed > FLIGHT_SPEED_MAX) {
                actor->speed = FLIGHT_SPEED_MAX;
            }
        } else if (actor->speed > FLIGHT_SPEED_MAX_PAL) {
            actor->speed = FLIGHT_SPEED_MAX_PAL;
        }
#else
        if (actor->speed > FLIGHT_SPEED_MAX) {
            actor->speed = FLIGHT_SPEED_MAX;
        }
#endif
        FIELDSTG_map.getFlyStep(&actor->tile, actor->speed, actor->dir, &move4);
        actor->pos.x += move4.x;
        actor->pos.y += move4.y;
        FIELDSTG_playStepSounds(actor, 1, 1);
        FIELDSTG_checkFlightProbes(actor);
        break;
    case ACTOR_STOP:
        runStop(actor);
        break;
    case ACTOR_WALK_OUT:
        switch (actor->step) {
        case 0:
        default:
            FIELDSTG_state.acting = 1;
            if (actor->flying == 0) {
                FIELDSTG_setActorAnim(actor, ACTOR_ANIM_RUN);
            }
            actor->nextStep(actor);
        case 1:
            break;
        }
        FIELDSTG_map.getWalkStep(&actor->tile, actor->speed, actor->dir, &move5);
        actor->pos.x += move5.x;
        actor->pos.y += move5.y;
        if (actor->flying == 0) {
            FIELDSTG_playStepSounds(actor, 1, 0);
        }
        break;
    case ACTOR_SLIDE:
        runSlide(actor);
        break;
    case ACTOR_STOP_SLIDE:
        runStopSlide(actor);
        break;
    case ACTOR_CLIMB:
        switch (actor->step) {
        case 0:
        default:
            FIELDSTG_setActorAnim(actor, ACTOR_ANIM_CLIMB);
            actor->nextStep(actor);
        case 1:
            break;
        }
        break;
    case ACTOR_CLIMB_UP:
        runClimbUp(actor);
        break;
    case ACTOR_CLIMB_DOWN:
        runClimbDown(actor);
        break;
    case ACTOR_GET_ON_WALL:
        runGetOnWall(actor);
        break;
    case ACTOR_GET_OVER_EDGE:
        switch (actor->step) {
        case 0:
        default:
            actor->hasShadow = 0;
            FIELDSTG_setActorAnim(actor, ACTOR_ANIM_GET_OVER_EDGE);
            actor->pos.x += actor->climbSide != 0 ? 0x1000 : -0x1000;
            actor->pos.y += 0x1800 + actor->wallHeight;
            FIELDSTG_followWithCamera(0, 2);
            actor->nextStep(actor);
        case 1:
            break;
        }
        if (actor->animDone == 0) {
            break;
        }
        actor->setSubstate(actor, ACTOR_CLIMB);
        actor->control = FIELDSTG_controlClimb;
        /* fallthrough */
    case ACTOR_POSED:
    default:
        actor->hasShadow = 1;
        break;
    case ACTOR_CLIMB_OFF_BOTTOM:
        runClimbOffBottom(actor);
        break;
    case ACTOR_CLIMB_OFF_TOP:
        runClimbOffTop(actor);
        break;
    case ACTOR_DROP:
        runDrop(actor);
        break;
    case ACTOR_GAUGE:
        runGauge(actor, children);
        break;
    case ACTOR_SEARCH:
        runSearch(actor);
        break;
    case ACTOR_TALK:
        runTalk(actor, children);
        break;
    case ACTOR_USE:
        runUse(actor);
        break;
    case ACTOR_USED:
        runUsed(actor);
        break;
    }
}
