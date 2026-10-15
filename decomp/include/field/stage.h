#ifndef STAGE_H
#define STAGE_H

/*
 * The stage overlays (AAA/PRO/WSTAG###.PRO), loaded at 0x800A4CA4 on top of
 * FIELDSTG. Each stage is its own small program; many share the same
 * functions, built from the same source. Those have the same name in every
 * stage (config/<version>/stages/<stage>.txt):
 *
 * - startStage(owner), FIELDSTG's start of the stage (StageEntry.init in
 *   FIELDSTG_stages, which links it as WSTAGnnn_startStage, include/field/stages.h):
 *   creates the StageTask, updateStage, and runs the stage's setup through
 *   its stageFuncs;
 * - setupStage(), the first of stageFuncs: fills FIELDSTG_state (FieldState)
 *   with the stage's file, text, map objects, triggers and battles;
 * - stepAnimation and its kin, the animation steppers.
 *
 * The ones whose copies are the same C are in src/field/stages/common/. A stage
 * with two copies of a function names the second with a 2.
 */

#include "engine/game.h"
#include "field/field_map.h"

/* The task a stage starts (startStage), updated by its updateStage */
typedef struct StageTask {
    TASK_HEADER(StageTask);
    /* 0x50 */ void *owner;
} StageTask;

/*
 * Carries the borrow down the digits of GAME.countdown, the timer of the
 * timed stages: a units digit that went below 0 (255) becomes 9 and takes one
 * from the digit above. The match depends on this statement macro's do-while.
 */
#define COUNTDOWN_BORROW(c)  \
    do {                     \
        if (c[2] >= 10) {    \
            c[2] = 9;        \
            c[1]--;          \
        }                    \
        if (c[1] >= 10) {    \
            c[1] = 9;        \
            c[0]--;          \
        }                    \
    } while (0)

/* Where an exit (StageSlot) leads: copyPlacePoints copies a place's points
   to the stage's first slots, field by field */
typedef struct StagePoint {
    /* 0x00 */ u16 arg; /* the mode */
    /* 0x02 */ u16 place;
    /* 0x04 */ u16 placeArg;
    /* 0x06 */ u16 x;
    /* 0x08 */ u16 y;
    /* 0x0A */ u16 dir;
    /* 0x0C */ struct StagePoint *next;
} StagePoint;

/* The points of a place (GAME.place and GAME.placeArg, which the exit the
   player came through set) */
typedef struct StagePoints {
    /* 0x00 */ s16 place;
    /* 0x02 */ s16 placeArg;
    /* 0x04 */ StagePoint *points;
} StagePoints;

/* A sprite of a StageEffect: its frame (0 for none) and palette row */
typedef struct StageSprite {
    /* 0x0 */ s16 frame;
    /* 0x2 */ s16 clutRow;
} StageSprite;

/*
 * A sprite at (x, y) from the stage's sheet (FieldState.sheetEntry)
 * with an animated palette, and a second one above it that a one-shot
 * animation adds to frame (what it is in the game isn't known yet)
 */
typedef struct StageEffect {
    TASK_HEADER(StageEffect);
    /* 0x50 */ s32 frame;
    /* 0x54 */ s32 x;
    /* 0x58 */ s32 y;
    /* 0x5C */ AnimState anim; /* the one-shot animation */
    /* 0x60 */ AnimState clutAnim;
    /* 0x64 */ StageSprite sprites[2]; /* drawn at depth 6 and 4 */
} StageEffect;

/*
 * The table of a stage's functions: its setup, then the menu stages' tween
 * (startTween and updateTween), which opens and closes a menu panel as the
 * menus' PanelAnim does (shared/start_fade.inc.c, update_fade.inc.c; the
 * stages' copy sets step before level)
 */
typedef struct StageFuncs {
    /* 0x0 */ void (*setup)(void); /* fills FIELDSTG_state */
    /* 0x4 */ void (*start)(PanelAnim *tween, s32 up);
    /* 0x8 */ s32 (*update)(PanelAnim *tween); /* whether it has ended */
} StageFuncs;

/* The task that animates the map objects */
typedef struct StageTileAnims {
    TASK_HEADER(StageTileAnims);
    /* 0x50 */ AnimState anims[4]; /* as many as the stage uses */
} StageTileAnims;

/* A map object with the animation that sets its frame */
typedef struct StageTileAnim {
    /* 0x0 */ StageTile *tile;
    /* 0x4 */ AnimState anim;
} StageTileAnim;

/*
 * Three map objects (animations 1 to 3) moved to
 * (x, y) and animated once when the substate is set to 1
 */
typedef struct StageTileEffect {
    TASK_HEADER(StageTileEffect);
    /* 0x50 */ s32 x;
    /* 0x54 */ s32 y;
    /* 0x58 */ StageTileAnim anims[3];
} StageTileEffect;

/* The children of a task with one per party slot */
typedef struct StagePartyChildren {
    /* 0x0 */ void *party[3];
} StagePartyChildren;

/*
 * A map object that wanders around where it starts,
 * animated, turning every period frames: towards home when it is more than
 * 30 away, else at random
 */
typedef struct StageWanderer {
    TASK_HEADER(StageWanderer);
    /* 0x50 */ s16 mode; /* 0: hidden, 1: wandering, 2: waiting, 3: slowing down */
    /* 0x52 */ u8 tileAnim; /* the map object (FieldState.objects) it moves */
    /* 0x53 */ u8 speedIndex;
    /* 0x54 */ s32 start; /* the first frame and timer */
    /* 0x58 */ s32 wait;
    /* 0x5C */ s32 homeX;
    /* 0x60 */ s32 homeY;
    /* 0x64 */ s32 x; /* 8.8 */
    /* 0x68 */ s32 y;
    /* 0x6C */ s16 posX; /* x and y >> 8 */
    /* 0x6E */ s16 posY;
    /* 0x70 */ s16 timer;
    /* 0x72 */ s16 period; /* frames between turns */
    /* 0x74 */ s16 speed;
    /* 0x76 */ s16 angle; /* 0x1000 a turn */
    /* 0x78 */ StageTileAnim tile;
} StageWanderer;

/* StageWanderer moving two map objects: tileAnim and tileAnim + 9 */
typedef struct StageWanderPair {
    TASK_HEADER(StageWanderPair);
    /* 0x50 */ s16 mode;
    /* 0x52 */ u8 tileAnim;
    /* 0x53 */ u8 speedIndex;
    /* 0x54 */ s32 start;
    /* 0x58 */ s32 wait;
    /* 0x5C */ s32 homeX;
    /* 0x60 */ s32 homeY;
    /* 0x64 */ s32 x;
    /* 0x68 */ s32 y;
    /* 0x6C */ s16 posX;
    /* 0x6E */ s16 posY;
    /* 0x70 */ s16 timer;
    /* 0x72 */ s16 period;
    /* 0x74 */ s16 speed;
    /* 0x76 */ s16 angle;
    /* 0x78 */ StageTileAnim tiles[2];
} StageWanderPair;

/*
 * Two map objects (animations 8 and 9) shown while
 * mode isn't 0, and hidden by a one-shot animation wait frames after mode 2
 */
typedef struct StageTileDuo {
    TASK_HEADER(StageTileDuo);
    /* 0x50 */ s16 mode;
    /* 0x52 */ s16 wait;
    /* 0x54 */ StageTileAnim tiles[2];
} StageTileDuo;

/*
 * Animates the map objects with animations 1 and 2:
 * the first alone while running, both when done
 */
typedef struct StageTilePair {
    TASK_HEADER(StageTilePair);
    /* 0x50 */ s32 playing; /* the second animation of the first map object */
    /* 0x54 */ s32 done; /* start in TASK_DONE */
    /* 0x58 */ StageTileAnim anims[2];
} StageTilePair;

/* StageTilePair with halfword flags */
typedef struct StageTilePair16 {
    TASK_HEADER(StageTilePair16);
    /* 0x50 */ s16 playing;
    /* 0x52 */ s16 done;
    /* 0x54 */ StageTileAnim anims[2];
} StageTilePair16;

/* StageTilePair with byte flags, for the map objects of two given animations */
typedef struct StageTilePairN {
    TASK_HEADER(StageTilePairN);
    /* 0x50 */ u8 playing;
    /* 0x51 */ u8 done; /* 1: start in TASK_DONE */
    /* 0x52 */ s16 anim; /* the map objects' animations: anim and anim + 1 */
    /* 0x54 */ StageTileAnim anims[2];
} StageTilePairN;

/*
 * A trap's effect over a partner it leaves at 1 HP (events 9000 to 9015 of
 * WSTAG740 and WSTAG741, on the Fire floor of the dungeon of traps): two sprites from the
 * stage's sheet at (x, y), each animated once, drawn 0xF0 above and 0x14
 * below
 */
typedef struct StageSpritePair {
    TASK_HEADER(StageSpritePair);
    /* 0x50 */ s32 pad50; /* never read or written */
    /* 0x54 */ s32 x;
    /* 0x58 */ s32 y;
    /* 0x5C */ AnimState anims[2];
    /* 0x64 */ StageSprite sprites[2];
} StageSpritePair;

/* A map object played through a sequence of animations */
typedef struct StageTileSeq {
    /* 0x0 */ s16 seq; /* the step playing, 0 when finished */
    /* 0x2 */ s16 setFrame; /* 0: the animation sets clutRow, else frame */
    /* 0x4 */ StageTile *tile;
    /* 0x8 */ AnimState anim;
} StageTileSeq;

/* The map objects with animations 1 to 4, sequenced */
typedef struct StageTileSeqs {
    TASK_HEADER(StageTileSeqs);
    /* 0x50 */ StageTileSeq entries[4];
} StageTileSeqs;

/* StageTileSeq with a byte setFrame and a sound to play at frame 0x34 */
typedef struct StageTileSeq8 {
    /* 0x0 */ s16 seq;
    /* 0x2 */ u8 sound; /* 1: play it, once */
    /* 0x3 */ u8 setFrame;
    /* 0x4 */ StageTile *tile;
    /* 0x8 */ AnimState anim;
} StageTileSeq8;

typedef struct StageTileSeqs8 {
    TASK_HEADER(StageTileSeqs8);
    /* 0x50 */ StageTileSeq8 entries[4];
} StageTileSeqs8;

/* A step of a StageTileSeq: its animation, and the step after it ends */
typedef struct StageSeqStep {
    /* 0x0 */ AnimFrame *frames;
    /* 0x4 */ s16 once; /* play once, then go to next */
    /* 0x6 */ s16 next; /* 0: hide the map object */
} StageSeqStep;

/* A frame of a StageTileFrame animation, which sets two frames of a map object */
typedef struct StageTileFrame {
    /* 0x0 */ u8 frame;
    /* 0x1 */ u8 duration;
    /* 0x2 */ u8 clutRow; /* for StageTile.clutRow */
    /* 0x3 */ u8 last; /* go to the next animation of the sequence after it */
} StageTileFrame;

/* Where a sequence of StageTileFrame animations (NULL-terminated) is at */
typedef struct StageTileCursor {
    /* 0x0 */ s16 seq;
    /* 0x2 */ s16 index;
    /* 0x4 */ s32 timer;
} StageTileCursor;

/*
 * The map object with animation 1, played once with a
 * sound when the substate is set to 1 (frame 5 before, 10 after)
 */
typedef struct StageSoundTile {
    TASK_HEADER(StageSoundTile);
    /* 0x50 */ s16 voice; /* what SOUND.playSound returned */
    /* 0x54 */ StageTileAnim obj;
} StageSoundTile;

/* A sprite effect a stage creates at (x, y) with the given frame, by kind */
typedef struct StageEffectSpot {
    /* 0x0 */ s16 frame;
    /* 0x2 */ s16 kind;
    /* 0x4 */ s32 x;
    /* 0x8 */ s32 y;
} StageEffectSpot;

/*
 * What moves a StageFaller or the StageMover of WSTAG415 at (x, y) in 8.8,
 * and its animation: one layout in both, whose animation the two step
 * functions, copies of each other, play (stepFallBody loops it,
 * stepMoverBody holds its last frame)
 */
typedef struct StageBody {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ s32 vx;
    /* 0x0C */ s32 vy;
    /* 0x10 */ s16 timer; /* the mover's frames of falling; a faller never sets it */
    /* 0x12 */ s16 mode; /* the mover's; a faller's is 1 once it has bounced */
    /* 0x14 */ AnimState anim;
} StageBody;

/* A sprite of WSTAG415 that falls from a point of a table, bounces once and dies at the bottom */
typedef struct StageFaller {
    TASK_HEADER(StageFaller);
    /* 0x50 */ s32 frame;
    /* 0x54 */ s32 x;
    /* 0x58 */ s32 y;
    /* 0x5C */ StageBody body;
} StageFaller;

/* Where a StageFaller starts and how it falls */
typedef struct StageFallParams {
    /* 0x0 */ AnimFrame *frames;
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 y;
    /* 0x8 */ s16 floorY;
    /* 0xA */ s16 vx;
    /* 0xC */ s16 gravity;
    /* 0xE */ s16 bounce; /* divides vy when bouncing, 0 for no bounce */
} StageFallParams;

/* The object of WSTAG415 that events 0x331 to 0x334 drive */
typedef struct StageMover {
    TASK_HEADER(StageMover);
    /* 0x50 */ s32 frame;
    /* 0x54 */ s32 x;
    /* 0x58 */ s32 y;
    /* 0x5C */ StageTile *tile; /* the map object (FieldState.objects) with animation 1 */
    /* 0x60 */ StageBody body;
} StageMover;

/* A scrolling background of two images of an archive, moving by (vx, -vy) */
typedef struct StageScroller {
    TASK_HEADER(StageScroller);
    /* 0x50 */ s32 x; /* 8.8 */
    /* 0x54 */ s32 y;
    /* 0x58 */ s32 vx;
    /* 0x5C */ s32 vy;
} StageScroller;

/* A map object, animated */
typedef struct StageTileTask {
    TASK_HEADER(StageTileTask);
    /* 0x50 */ StageTileAnim obj;
} StageTileTask;

/* StageTileDuo with one map object */
typedef struct StageTileSolo {
    TASK_HEADER(StageTileSolo);
    /* 0x50 */ s16 mode;
    /* 0x52 */ s16 wait;
    /* 0x54 */ StageTileAnim tile;
} StageTileSolo;

/* The map object with the given animation, moved to (x, y) */
typedef struct StageTileAt {
    TASK_HEADER(StageTileAt);
    /* 0x50 */ s32 x;
    /* 0x54 */ s32 y;
    /* 0x58 */ s32 anim;
    /* 0x5C */ StageTileAnim obj;
} StageTileAt;

/* The map object with animation 1 and a frame counter */
typedef struct StageTileTimer {
    TASK_HEADER(StageTileTimer);
    /* 0x50 */ StageTile *tile;
    /* 0x54 */ s32 timer;
} StageTileTimer;

/* The map objects with animations 2 to 9 */
typedef struct StageTileGroup {
    TASK_HEADER(StageTileGroup);
    /* 0x50 */ StageTile *tiles[8];
} StageTileGroup;

/* A frame of a StageRiser's animation; the last (frame 0xFF) sets move */
typedef struct StageRiserFrame {
    /* 0x0 */ s16 frame;
    /* 0x2 */ u8 duration;
    /* 0x3 */ u8 move;
} StageRiserFrame;

/* A map object animated and moved by its animation */
typedef struct StageRiser {
    /* 0x00 */ s32 y; /* 8.8 */
    /* 0x04 */ s32 move; /* added to the map object's y */
    /* 0x08 */ s16 active;
    /* 0x0A */ s16 tileAnim;
    /* 0x0C */ AnimState anim;
    /* 0x10 */ StageTile *tile;
} StageRiser;

/* A map object that moves by a speed in 8.8 */
typedef struct StageLift {
    /* 0x0 */ s16 active;
    /* 0x2 */ s16 speed;
    /* 0x4 */ StageTile *tile;
} StageLift;

/* Map objects that events start moving */
typedef struct StageRisers {
    TASK_HEADER(StageRisers);
    /* 0x050 */ StageRiser risers[11];
    /* 0x12C */ StageLift lifts[3];
    /* 0x144 */ s32 timer;
    /* 0x148 */ s16 pushing; /* moves the player */
    /* 0x14A */ s16 push;
} StageRisers;

/* A map object animated while playing is set */
typedef struct StageTileAnimFlag {
    /* 0x0 */ StageTile *tile;
    /* 0x4 */ s32 playing;
    /* 0x8 */ AnimState anim;
} StageTileAnimFlag;

/* Six map objects, animated by mode */
typedef struct StageTileSix {
    TASK_HEADER(StageTileSix);
    /* 0x50 */ s32 mode;
    /* 0x54 */ StageTileAnim tiles[6];
} StageTileSix;

/* StageTileSix with flagged map objects */
typedef struct StageTileSixW {
    TASK_HEADER(StageTileSixW);
    /* 0x50 */ s32 mode;
    /* 0x54 */ StageTileAnimFlag tiles[6];
} StageTileSixW;

/* A sprite that flies off diagonally, animated */
typedef struct StageFlyer {
    TASK_HEADER(StageFlyer);
    /* 0x50 */ s32 up; /* else down */
    /* 0x54 */ s32 right; /* else left; also flips the sprite */
    /* 0x58 */ s32 still; /* doesn't move or animate */
    /* 0x5C */ s32 anim;
    /* 0x60 */ s32 frame;
    /* 0x64 */ s32 timer;
    /* 0x68 */ s32 x; /* 8.8 */
    /* 0x6C */ s32 y;
} StageFlyer;

/* A brightness that rises from 12 to 28, holds, then falls back */
typedef struct StageGlow {
    TASK_HEADER(StageGlow);
    /* 0x50 */ s32 level;
} StageGlow;

/* An animation played at (x, y) */
typedef struct StageAnimSpot {
    /* 0x0 */ AnimFrame *frames;
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 y;
} StageAnimSpot;

/* Two map objects, animated */
typedef struct StageTileLoop2 {
    TASK_HEADER(StageTileLoop2);
    /* 0x50 */ StageTileAnim tiles[2];
} StageTileLoop2;

/* Two pictures that change by turns, and the flyers they let out */
typedef struct StageFlyerGate {
    TASK_HEADER(StageFlyerGate);
    /* 0x50 */ s32 left; /* the picture of the first two (0-2) */
    /* 0x54 */ s32 right; /* the picture of the third (0-2) */
    /* 0x58 */ s32 spawnA; /* let out the first set of flyers */
    /* 0x5C */ s32 spawnB; /* let out the second set */
} StageFlyerGate;

/* An animation of a table of byte pairs (frame, duration; duration 0 loops to frame) */
typedef struct StageByteAnim {
    /* 0x00 */ s32 anim; /* the animation wanted */
    /* 0x04 */ s32 cur; /* the animation playing */
    /* 0x08 */ s32 index;
    /* 0x0C */ s32 frame;
    /* 0x10 */ s32 timer;
} StageByteAnim;

/* Two StageByteAnim animations */
typedef struct StageByteAnims {
    TASK_HEADER(StageByteAnims);
    /* 0x50 */ StageByteAnim anims[2];
} StageByteAnims;

/* A quad of the screen: its corners and what it shows */
typedef struct StageQuad {
    /* 0x00 */ s16 x0;
    /* 0x02 */ s16 y0;
    /* 0x04 */ s16 x1;
    /* 0x06 */ s16 y1;
    /* 0x08 */ s16 x2;
    /* 0x0A */ s16 y2;
    /* 0x0C */ s16 x3;
    /* 0x0E */ s16 y3;
    /* 0x10 */ s16 kind; /* 0-1: the frame of that animation, else the texture */
} StageQuad;

/*
 * A texture of a StageQuad: its page, square at (u, v), and palette. vramX
 * and vramY, the square's place in VRAM (tpageX + (u & 0xFF) / 4, tpageY +
 * v, the 4-bit texels four to a halfword), are in the table but nothing reads
 * them.
 */
typedef struct StageQuadTexture {
    /* 0x00 */ s16 tpageX;
    /* 0x02 */ s16 tpageY;
    /* 0x04 */ s16 vramX; /* unused */
    /* 0x06 */ s16 vramY; /* unused */
    /* 0x08 */ s16 u;
    /* 0x0A */ s16 v;
    /* 0x0C */ u16 clutX;
    /* 0x0E */ u16 clutY;
} StageQuadTexture;

/* The map objects with animations 3 and 4, each animated once */
typedef struct StageTileOnce {
    TASK_HEADER(StageTileOnce);
    /* 0x50 */ s32 pad50; /* never read or written */
    /* 0x54 */ StageTileAnimFlag tiles[2];
} StageTileOnce;

/* A frame of a looping animation: after the last (frame 0xFF) it goes back to loop */
typedef struct StageTileLoopFrame {
    /* 0x0 */ s16 frame;
    /* 0x2 */ u8 duration;
    /* 0x3 */ u8 loop;
} StageTileLoopFrame;

/* A frame of a StageFloaterPart's animation, with what it adds to value */
typedef struct StageFloaterFrame {
    /* 0x0 */ s16 frame;
    /* 0x2 */ s16 duration;
    /* 0x4 */ s32 delta;
} StageFloaterFrame;

/* A sprite of a StageFloater, its animation and what the animation adds up */
typedef struct StageFloaterPart {
    /* 0x0 */ s32 value;
    /* 0x4 */ StageSprite sprite;
    /* 0x8 */ AnimState anim;
} StageFloaterPart;

/* A sprite from the stage's sheet at (x, y) that moves up or down in TASK_DONE */
typedef struct StageFloater {
    TASK_HEADER(StageFloater);
    /* 0x50 */ s32 x;
    /* 0x54 */ s32 y;
    /* 0x58 */ s32 y8; /* y << 8 while moving */
    /* 0x5C */ s32 up;
    /* 0x60 */ StageFloaterPart parts[3];
} StageFloater;

/* Where a StageFloater is created */
typedef struct StageFloaterSpot {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 up;
} StageFloaterSpot;

/* Sets its children to TASK_DONE one after the other, at the times of a table */
typedef struct StageFloaterChain {
    TASK_HEADER(StageFloaterChain);
    /* 0x50 */ s16 next;
    /* 0x52 */ s16 timer;
} StageFloaterChain;

/* The children of a StageFloaterChain */
typedef struct StageFloaters {
    /* 0x00 */ StageFloater *floaters[27];
} StageFloaters;

/* The map objects with animations 1 to 4 */
typedef struct StageTileQuad {
    TASK_HEADER(StageTileQuad);
    /* 0x50 */ StageTileAnim anims[4];
} StageTileQuad;

/* The map objects with animations 4 to 1, animated */
typedef struct StageTileSet {
    TASK_HEADER(StageTileSet);
    /* 0x50 */ s32 done; /* start in TASK_DONE */
    /* 0x54 */ StageTile *tiles[4]; /* animations 4, 3, 2 and 1 */
    /* 0x64 */ AnimState anim;
    /* 0x68 */ AnimState anim2; /* animation 1's, looping */
} StageTileSet;

/* A sprite drawn over a character (FIELDSTG's actor found by its key) */
typedef struct StageActorMark {
    TASK_HEADER(StageActorMark);
    /* 0x50 */ s32 kind;
    /* 0x54 */ Actor *actor;
} StageActorMark;

/* A frame the task sets on a map object */
typedef struct StageFrameTask {
    TASK_HEADER(StageFrameTask);
    /* 0x50 */ s32 frame;
    /* 0x54 */ u8 pad54[8]; /* never read or written */
} StageFrameTask;

/* The map objects with animations 1 to 5, each playing a sequence */
typedef struct StageTileCursors {
    TASK_HEADER(StageTileCursors);
    /* 0x50 */ StageTileCursor cursors[5];
} StageTileCursors;

/*
 * The map object with animation 2, animated by
 * substate, and the StageTileAt children that events set to TASK_DONE
 */
typedef struct StageTileSwitch {
    TASK_HEADER(StageTileSwitch);
    /* 0x50 */ s32 spawn; /* the child to set to TASK_DONE, + 1 */
    /* 0x54 */ s32 silent; /* no sound when an event comes */
    /* 0x58 */ StageTileAnim obj;
} StageTileSwitch;

/* Where a StageTileAt is created */
typedef struct StageTileAtSpot {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 anim;
} StageTileAtSpot;

/* The children of a StageTileSwitch */
typedef struct StageTileAts {
    /* 0x00 */ StageTileAt *tiles[5];
} StageTileAts;

/* A looping animation of a table of byte pairs (frame, duration; frame 0 loops) */
typedef struct StageBytePlay {
    /* 0x0 */ s32 frame;
    /* 0x4 */ s32 index;
    /* 0x8 */ s32 timer; /* counts up */
} StageBytePlay;

/* The animations of sprites drawn all over the stage */
typedef struct StageSpriteField {
    TASK_HEADER(StageSpriteField);
    /* 0x50 */ StageBytePlay anims[10]; /* nine used */
} StageSpriteField;

/* A sprite of the stage: where, which animation, and flipped */
typedef struct StageSpriteSpot {
    /* 0x0 */ s16 x; /* 0 ends the table */
    /* 0x2 */ s16 y;
    /* 0x4 */ u8 anim;
    /* 0x5 */ u8 flip;
} StageSpriteSpot;

/* A yes/no menu at the bottom of the screen, a copy of FIELDSTG's ChoiceTask
   (FIELDSTG_runChoice) without its type; its children are a ChoiceChildren */
typedef struct StageMenu {
    TASK_HEADER(StageMenu);
    /* 0x50 */ s32 cursor;
    /* 0x54 */ PanelAnim tween;
} StageMenu; /* 0x64 */


/* A list menu whose entries show a message */
typedef struct StageListMenu {
    TASK_HEADER(StageListMenu);
    /* 0x50 */ s32 count;
    /* 0x54 */ s32 cursor;
    /* 0x58 */ s32 showArrow;
    /* 0x5C */ s32 arrowFrame;
    /* 0x60 */ s32 arrowTime;
    /* 0x64 */ PanelAnim tweens[2]; /* list, message box */
} StageListMenu; /* 0x84 */

typedef struct StageListMenuChildren {
    /* 0x00 */ TextWindow *options[8];
    /* 0x20 */ Cursor *cursor;
    /* 0x24 */ TextWindow *message;
} StageListMenuChildren; /* 0x28 */

/*
 * The menu sprite sheet, and an entry of a text file: the European version
 * numbers the disc's files differently, and has a copy of each text file per
 * language, one after the other, so it adds the language (LANGUAGE, which
 * CNTY_SEL sets) to the file. The Japanese version numbers them as the USA
 * one, and its text files are the Japanese copies (a stage's text defines
 * are TEXT_FILE of the USA version's).
 */
#if VERSION_US || VERSION_JP
#define MENU_SPRITES 0x2770000
#elif VERSION_EU
#define MENU_SPRITES 0x2860000
#endif
#if VERSION_US
#define TEXT_ENTRY(file, n) ((file) << 16 | (n))
#elif VERSION_EU || VERSION_JP
#define TEXT_ENTRY(file, n) ((LANGUAGE << 16) + ((file) << 16 | (n)))
#endif

/*
 * The text file of the extra chapter's (WSTAG920-974) menus and events, to
 * which TEXT_ENTRY adds the language: the European version's copies of it, one
 * per language, start with the Japanese one (COUNTRY/JPN/MSDMG900.BIN), which
 * is the Japanese version's
 */
#if VERSION_EU
#define EXTRA_TEXT 0x158
#elif VERSION_JP
#define EXTRA_TEXT 0x939
#endif

/*
 * The files a stage's setup function gives FIELDSTG, numbered differently in
 * each version, are the stage's own defines: STAGE_TEXT, its text file (the
 * European version adds the language), and STAGE_FILE, the file whose
 * entries go to FIELDSTG_map.setFile (STAGE_FILE << 16 | n), its neighbours
 * usually the files at mapFile and imageFile (STAGE_FILE_8 and STAGE_ARCHIVE
 * otherwise). EVENT_TEXT_FILE is the text file of the stage's events,
 * counted from TEXT_FILE(1) as FieldEvent.text is.
 */
/* An entry of the stage's events' text file, for FieldEvent.text */
#define EVENT_TEXT(n) (EVENT_TEXT_FILE << 16 | (n))

/* The sounds the stages play, with SOUNDTST's names (engine/sound.h has the
   ones the overlays share) */
#define SOUND_MTL_DOWN 0x40012
#define SOUND_COMAT102 0x8004213E
#define SOUND_COMAT103 0x800421BF
#define SOUND_COMEX105 0x800442C1
#define SOUND_COMEX112 0x80044648
#define SOUND_COMEX114 0x8004474A
#define SOUND_BULB_000 0x340001
#define SOUND_BULB_001 0x340002
#define SOUND_GONDRA_B 0x340003
#define SOUND_BULB_002 0x4C0001
#define SOUND_FLOOR_LT 0x4C0002
#define SOUND_DIGITAMA 0x540001
#define SOUND_SE000000 0xA40004
#define SOUND_DOOROPEN 0x80A4203C
#define SOUND_LOGINEF 0xCC0001
#define SOUND_FLOOR_DN 0xEC0001
#define SOUND_ENTRY_02 0x1000000
#define SOUND_SIGNALON 0x1000002
#define SOUND_DOORCLSE 0x8100303C
#define SOUND_CCOMBINE 0x8100383C
#define SOUND_DIGI_EN0 0x10C0000
#define SOUND_DIGI_EN1 0x10C0001
#define SOUND_DIGI_EN2 0x10C0002


/*
 * The names every stage defines, each its own (config/<version>/stages/<stage>.txt).
 * updateStage takes the task, and in some stages its children too.
 */
void updateStage();
void setupStage(void);

/* The stage's functions (common/start_stage.inc.c calls the first): a
   StageFuncs in the stages with a tween, which define STAGE_TWEEN before
   they include this file, and in the others an array of just setupStage */
#ifdef STAGE_TWEEN
extern StageFuncs stageFuncs;
#else
extern void (*stageFuncs[])(void);
#endif

/* The tables and color setupStage gives FIELDSTG_state (FieldState) */
extern StageTile stageObjects[];
extern StageSlot stageSlots[];
extern ActorImage stageImages[];
extern FieldActorEntry *stageActors[];
extern const CVECTOR stageColor;
extern FieldEvent stageEvents[];
extern FieldBattles stageBattles[];

/* Names some stages define */
extern StagePoints *placePoints[];
extern AnimFrame effectFrames[];
extern AnimFrame effectClutFrames[];
StageEffect *createStageEffect(s32 x, s32 y, s32 frame);
void updateTileAnims(StageTileAnims *task);

#endif /* STAGE_H */
