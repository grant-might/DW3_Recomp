#ifndef FIELD_MAP_H
#define FIELD_MAP_H

/*
 * What the stage overlays and the executable use of FIELDSTG (fieldstg.h has
 * the rest): its layers and task ids, the map and the functions that read
 * it, the records of the tables that each stage gives FIELDSTG, the field's
 * state, and the functions the stages and the executable call.
 * The map is a tree of cells: a grid of 128-pixel cells, then levels of 64,
 * 32, 16 and 8 pixels, each cell four of the next level's, then a byte for
 * each pixel of the 8x8 blocks.
 */

#include "engine/game.h"

/* The field's drawing layers, by their ids (GFX.funcs.createLayer), as
   FIELDSTG_updateField creates them */
#define FIELD_LAYER_BACK 0x1000 /* the background's color */
#define FIELD_LAYER_COVER 0x1001 /* the cover that fades the screen (FIELDSTG_drawCover) */
#define FIELD_LAYER_MAP 0x1002 /* the map, its objects and characters, and the menus */
#define FIELD_LAYER_BANNER 0x1003 /* the area name banner */
#define FIELD_LAYER_TEXT 0x1004 /* the message and talk boxes */

/* The ids of the field's tasks (createTaskWithId, TASK_REGISTRY.funcs.find) */
#define FIELD_TASK_MAP 4 /* FIELDSTG_createMapStreamer's */
#define FIELD_TASK_ACTOR 5 /* every character (FIELDSTG_createActor) */
#define FIELD_TASK_FIELD 7 /* the field's main task (FIELDSTG_createField) */
#define FIELD_TASK_BANNER 9 /* the area name banner */
#define FIELD_TASK_HIDDEN_SPOTS 0xB
#define FIELD_TASK_CAMERA 0x10
#define FIELD_TASK_ICON 0x16 /* the icon over the player's head */
#define FIELD_TASK_LAUNCHER 0x17 /* a stage's launchers (FIELDSTG_runLaunch) */
#define FIELD_TASK_COMMANDS 0x32D /* script command 813 (FIELDSTG_startCommandTask): the field commands */

/*
 * The field commands, which an event script hands FIELD_TASK_COMMANDS as the
 * first argument of a pose command (FIELDSTG_handleFieldCommand). The
 * PLAY_ commands play the sound of their SOUNDTST name; a STOP_ command
 * keys off the sound its PLAY_ command holds.
 */
#define FIELD_COMMAND_HALT_PARTNERS 0x337 /* the partners stop following */
#define FIELD_COMMAND_ICON1 0x338 /* the player icon's substate 1, with ITEM_GET */
#define FIELD_COMMAND_ICON2 0x339 /* its substate 2 */
#define FIELD_COMMAND_ICON3 0x34A /* its substate 3, with ITEM_GET */
#define FIELD_COMMAND_HIDE_OBJECTS(n) (0x34D + (n)) /* hides the map objects of group n */
#define FIELD_COMMAND_SHOW_OBJECTS(n) (0x353 + (n)) /* shows them */
#define FIELD_OBJECT_GROUPS 6
#define FIELD_OBJECT_GROUP_ANIM 100 /* the StageTile.anim of group 0; group n has 100 + n */
#define FIELD_COMMAND_PLAY_INFO_SIG 0x365
#define FIELD_COMMAND_PLAY_GAYALOOP 0x366
#define FIELD_COMMAND_STOP_GAYALOOP 0x367
#define FIELD_COMMAND_PLAY_WEAR_OFF 0x368
#define FIELD_COMMAND_PLAY_DEMO_BGM 0x369
#define FIELD_COMMAND_PLAY_SE000002 0x36A
#define FIELD_COMMAND_PLAY_BEAM_SHT 0x36B
#define FIELD_COMMAND_PLAY_SWITCH02 0x36C
#define FIELD_COMMAND_PLAY_MASK_SET 0x36D
#define FIELD_COMMAND_PLAY_BM_ERASE 0x36E
#define FIELD_COMMAND_PLAY_LD_ERASE 0x36F
#define FIELD_COMMAND_PLAY_PLAYER11 0x370
#define FIELD_COMMAND_STOP_PLAYER11 0x371
#define FIELD_COMMAND_SHAKE_CAMERA 0x372
#define FIELD_COMMAND_STOP_CAMERA_SHAKE 0x373
#define FIELD_COMMAND_PLAY_TRAP_OFF 0x374
#define FIELD_COMMAND_PLAY_SAVEDEMO 0x375
#define FIELD_COMMAND_SEARCH_EVENT_SPOT 0x376 /* FIELDSTG_searchEventSpot */
#define FIELD_COMMAND_PLAY_SWITCH03 0x377
#define FIELD_COMMAND_PLAY_SN_ENTRY 0x378
#define FIELD_COMMAND_PLAY_SN_ERASE 0x379
#define FIELD_COMMAND_PLAY_TELEPORT 0x37A
#define FIELD_COMMAND_PLAY_BULB_003 0x37C
#define FIELD_COMMAND_PLAY_GONDRA_S 0x37D
#define FIELD_COMMAND_PLAY_PIYOPIYO 0x37E
#define FIELD_COMMAND_PLAY_COMCD103 0x37F
#define FIELD_COMMAND_PLAY_COMCD201 0x380
#define FIELD_COMMAND_PLAY_COMCD111 0x381
#define FIELD_COMMAND_PLAY_BEAM_HIT 0x382
#define FIELD_COMMAND_PLAY_SWITCH01 0x383
#define FIELD_COMMAND_PLAY_COMCD115 0x384
#define FIELD_COMMAND_STOP_COMCD115 0x385
#define FIELD_COMMAND_STOP_BEAM_HIT 0x386

/* Some of FieldMap's maps (files): those of the floors the player walks on
   come first, and GAME.mapIndex picks one (FieldMap.setMap, SLOT_MAP) */
#define FIELD_MAP_FLOOR0 0
#define FIELD_MAP_FLOOR1 1
#define FIELD_MAP_AREAS 4 /* a cell's value: its battle area (FieldBattles), or 0 */
#define FIELD_MAP_TRIGGERS 7 /* a cell's value: a direction (3 bits) and a StageSlot (5) */

typedef struct FieldMap {
    /* 0x00 */ s32 files[8]; /* the file entry of each map, set by setFile */
    /* 0x20 */ s32 width; /* of the grid, in cells */
    /* 0x24 */ s32 height;
    /* 0x28 */ u8 *grid;
    /* 0x2C */ u8 *cells64;
    /* 0x30 */ s16 *cells32;
    /* 0x34 */ s16 *cells16;
    /* 0x38 */ s16 *cells8;
    /* 0x3C */ u8 *pixels;
    /* 0x40 */ void (*setFile)(s32 index, s32 file); /* FIELDSTG_setMapFile */
    /* 0x44 */ s32 (*getCell)(s32 index, Vec2 *pos); /* FIELDSTG_getMapCell */
    /* 0x48 */ void (*getWalkStep)(Vec2 *pos, s32 scale, s32 dir, Vec2 *out); /* FIELDSTG_getWalkStep */
    /* 0x4C */ void (*getFlyStep)(Vec2 *pos, s32 scale, s32 dir, Vec2 *out); /* FIELDSTG_getFlyStep */
    /* 0x50 */ void (*setFirstMap)(s32 index); /* FIELDSTG_setFirstMap: the map the player starts on
                                                  (GAME.mapIndex), when the mode is new */
    /* 0x54 */ void (*setMap)(s32 index); /* FIELDSTG_setMap: the map the player is on */
    /* 0x58 */ s32 (*isTileFree)(Vec2 *pos); /* FIELDSTG_isTileFree: 0 where a character or an object stands */
} FieldMap;

extern FieldMap FIELDSTG_map;

/* Where an actor's frames go in VRAM: one of the records after the
   FieldImage (Actor.image) */
typedef struct ActorImage {
    /* 0x0 */ s16 x; /* the texture page */
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 imageX; /* where FIELDSTG_animateActor loads each frame */
    /* 0x6 */ s16 imageY;
    /* 0x8 */ s16 u;
    /* 0xA */ s16 v;
    /* 0xC */ s16 clutX;
    /* 0xE */ s16 clutY;
} ActorImage;

/* The image of the field's effects (FieldState.images),
   then the actors' ActorImages */
typedef struct FieldImage {
    /* 0x00 */ ActorImage shadow; /* the actors' shadow (FIELDSTG_drawActor) */
    /* 0x10 */ s16 x; /* in VRAM */
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 w;
    /* 0x16 */ s16 h;
    /* 0x18 */ s16 u;
    /* 0x1A */ s16 v;
    /* 0x1C */ s16 clutX;
    /* 0x1E */ s16 clutY;
} FieldImage;

/* An event of the table FieldState.events, up to
   the first id -1, which FIELDSTG_startEvent starts */
typedef struct FieldEvent {
    /* 0x00 */ s32 id;
    /* 0x04 */ s16 *script; /* FIELDSTG_runEvent's, or NULL */
    /* 0x08 */ s32 text; /* a file and index, the file counted from
                            TEXT_FILE(1); or 0 */
    /* 0x0C */ void *(*start)(void); /* without a script: the task it starts, or NULL */
    /* 0x10 */ void (*end)(void); /* or NULL */
} FieldEvent;

/*
 * The commands of an event script (FieldEvent.script), as FIELDSTG_runEvent
 * reads them: a halfword kind << 8 | variant, then a fixed number of
 * arguments. x and y are in pixels, ids are the characters' (Actor.key1)
 * below 0x320 and the script commands' from 0x320 on.
 */
#define SCRIPT_END 0 /* ends the event (and any unknown kind) */
#define SCRIPT_PLACE(id, x, y) 0x100, id, x, y /* puts a character at (x, y) */
#define SCRIPT_POSE(id, set, dir) 0x101, id, set, dir /* Actor.setPose: its animation set, facing dir */
/* the same word for a script command's id: FIELDSTG_setEventPose creates its task
   if none runs and hands it the command and the argument (FIELDSTG_handleScriptCommand) */
#define SCRIPT_COMMAND(id, command, arg) 0x101, id, command, arg
#define SCRIPT_WALK(id, x, y, dir) 0x102, id, x, y, dir /* Actor.setGoal: walks to (x, y), then faces dir */
/* box of the event's boxes shows the event text's entry: a talk box at the corner
   kind next to the character id, or for kind 4 a message box (FIELDSTG_createSpeech) */
#define SCRIPT_TALK(box, entry, id, kind) 0x200, box, entry, id, kind
#define SCRIPT_WAIT(frames) 0x300, frames
#define SCRIPT_WAIT_BOX 0x301 /* until box 0 closes */
#define SCRIPT_WAIT_WALK(id) 0x302, id /* until the character gets to its goal */
#define SCRIPT_WAIT_ANIM(id) 0x303, id /* until its animation ends */
#define SCRIPT_LEAVE(mode, x, y, dir) 0x304, mode, x, y, dir /* FIELDSTG_leaveField; ends the event */
#define SCRIPT_FOLLOW(snap, id) 0x600, snap, id /* FIELDSTG_followWithCamera */
#define SCRIPT_LOOK_AT(snap, x, y) 0x601, snap, x, y /* FIELDSTG_pointCamera */

/* What a character says (FIELDSTG_runActorAction): the first whose conditions hold,
   or the last, which has none */
typedef struct FieldTalk {
    /* 0x0 */ u16 *conditions; /* FLAGS_00.checkConditions's */
    /* 0x4 */ u16 *actions; /* FLAGS_00.applyActions's when it ends, or NULL */
    /* 0x8 */ s32 entry; /* its text's entry in the field's text file (FIELDSTG_createTalk) */
} FieldTalk;

/* The characters (Actor.key1, FieldActorEntry.id) that FIELDSTG_updateField
   creates for the party. A field can list one of the player's other
   characters, 1, 0x6A, 0x146 or 0x147, to lead instead, without partners. */
#define FIELD_CHARACTER_PLAYER 2
#define FIELD_CHARACTER_PARTNERS 3 /* + the partner (GAME.funcs.getPartyPartner) */

/* The FLAGS_00 flag that every encounter sets (FIELDSTG_startEncounter);
   until it is set, the stages' slots run an event before the first battle */
#define FIELD_FLAG_ENCOUNTERED 0xF

/* A character of the field (FieldState.actors, a list of pointers up to NULL), which FIELDSTG_updateField creates
   unless its conditions fail; the player's other characters (FIELD_CHARACTER_PLAYER) lead the party instead. */
typedef struct FieldActorEntry {
    /* 0x00 */ u16 *conditions; /* FLAGS_00.checkConditions's, or NULL */
    /* 0x04 */ struct FieldTalk *talks; /* up to the first without conditions */
    /* 0x08 */ s16 id;
    /* 0x0A */ s16 image; /* its ActorImage, of FieldState.images.actors from 2 (FIELDSTG_createActor) */
    /* 0x0C */ s16 x; /* in pixels */
    /* 0x0E */ s16 y;
    /* 0x10 */ s16 dir;
} FieldActorEntry;

/*
 * An object of a map, which FIELDSTG draws: a record of the table at
 * FieldState.objects, which ends with margin = 0
 */
typedef struct StageTile {
    /* 0x00 */ u8 visible;
    /* 0x01 */ u8 anim; /* which of a stage's animations sets frame (1-3);
                           FIELDSTG finds objects by it (its lift, 2 and 3,
                           the field commands' groups, FIELD_OBJECT_GROUP_ANIM
                           on) and draws 0xFF from its effect sprites */
    /* 0x02 */ u8 margin; /* how far off the view it is still drawn; 0 ends the table */
    /* 0x03 */ u8 depth;
    /* 0x04 */ u8 frame;
    /* 0x05 */ u8 cycle; /* 1 cycles the frame, 2 the CLUT row, 3 the row back and forth */
    /* 0x06 */ u8 cycleFirst;
    /* 0x07 */ u8 cycleLast;
    /* 0x08 */ u8 cycleDelay;
    /* 0x09 */ u8 clutRow;
    /* 0x0A */ s16 x;
    /* 0x0C */ s16 y;
    /* 0x0E */ s16 sortY; /* drawn sorted at this y, or 0 */
    /* 0x10 */ s16 cycleTime; /* in 1/256 frames; bit 15: going back */
} StageTile;

/*
 * The lift: the map objects 3 (always shown) and 2 (shown while it moves),
 * which move 0x7F pixels up or down the screen with the player, shaking
 * before and after, each time a script command sets TASK_DONE: FIELDSTG's
 * (FIELDSTG_createLift, script command 826) and the copies of WSTAG261 and
 * WSTAG934 (common/update_tile_lift.inc.c)
 */
#define LIFT_UP 0x348 /* the lift's script commands: up the screen, */
#define LIFT_DOWN 0x349 /* and back down to the objects' place on the map */

typedef struct Lift {
    TASK_HEADER(Lift);
    /* 0x50 */ StageTile *tiles[2]; /* the map objects 3 and 2 */
    /* 0x58 */ s16 raised; /* the objects are 0x7F up from their place, and the next
                              move takes them down (LIFT_DOWN; flag 0x1C3D at the start) */
    /* 0x5A */ s16 timer;
    /* 0x5C */ s16 shake; /* the step of the shake table, then a frame count */
    /* 0x5E */ s16 pad5E; /* never read or written */
    /* 0x60 */ s16 y[2]; /* the objects' y when the move started */
    /* 0x64 */ s32 playerY;
    /* 0x68 */ s16 homeY[2]; /* the objects' place on the map */
} Lift;

/* Points of the story (GAME.progress) where the field and the stages act differently */
#define FIELD_PROGRESS_MOVIE_BATTLES 0x2B /* each encounter plays MODE_BATTLE_MOVIE first */
#define FIELD_PROGRESS_EXTRA 0x2D /* the European version's extra chapter, whose stages are
                                     FIELDSTG_extraStages (WSTAG920 to WSTAG974) */

/* The kinds of StageSlot (type). SLOT_DEPTH, SLOT_MAP, SLOT_EVENT, the slides
   and SLOT_LAUNCH act as the player steps on them; the others show a balloon
   and wait for cross, and those up to SLOT_GAUGE only while the player faces
   them (FIELDSTG_findTrigger) */
#define SLOT_EXIT 1 /* leaves for mode arg at (x, y), facing dir */
#define SLOT_CLIMB_UP 2
#define SLOT_CLIMB_DOWN 3
#define SLOT_DROP 4
#define SLOT_DEPTH 5 /* the player's depth, arg */
#define SLOT_MAP 6 /* the map the player is on, arg (FieldMap.setMap) */
#define SLOT_GAUGE 7 /* the gauge game */
#define SLOT_EVENT 8 /* starts the event arg */
#define SLOT_WARP1 9 /* a warp (FieldWarp at arg) with effect and cutscene 1 */
#define SLOT_WARP0 10 /* the same with effect and cutscene 0 */
#define SLOT_SLIDE 11
#define SLOT_STOP_SLIDE 12
#define SLOT_LAUNCH 13 /* sends the player flying (FIELDSTG_launchActor) */
#define SLOT_LAUNCH_OUT 14 /* the same, then leaves for mode arg */

/*
 * What the player can trigger on a map (FIELDSTG_updateTriggers): a record
 * of the table at FieldState.slots, which ends with type 0,
 * where the points are copied to. The stages fill arg to placeArg
 * from their place points (copyPlacePoints, in src/field/stages/common).
 */
typedef struct StageSlot {
    /* 0x00 */ u16 conditions[2][2]; /* flag code and value, or code 0xFFFF */
    /* 0x08 */ u16 type; /* SLOT_EXIT... */
    /* 0x0A */ u16 arg; /* by type: a mode, a depth, a map, an event or a height (in 16
                           pixels, plus 1 for a climb); a warp's and a launch's SlotDest starts here */
    /* 0x0C */ u16 x;
    /* 0x0E */ u16 y;
    /* 0x10 */ u16 dir;
    /* 0x12 */ u16 hideAnim; /* the animation of the map objects to hide, or 0 */
    /* 0x14 */ u16 place; /* copied to GAME.place, the place (FieldBattles.id) */
    /* 0x16 */ u16 placeArg; /* copied to GAME.placeArg: with place, the next stage's place points */
} StageSlot;

/* Where a slot leads: a StageSlot from its arg on, as the warps (FieldTask.warp)
   and the launches (Launch.dest) read it */
typedef struct SlotDest {
    /* 0x0 */ s16 mode;
    /* 0x2 */ s16 x; /* in pixels */
    /* 0x4 */ s16 y;
    /* 0x6 */ s16 dir;
    /* 0x8 */ u16 hideAnim; /* which a warp doesn't read */
    /* 0xA */ u16 place; /* copied to GAME.place, the place (FieldBattles.id) */
    /* 0xC */ u16 placeArg; /* copied to GAME.placeArg */
} SlotDest;

/* A slot's destination: its arg on, read as signed halfwords */
#define SLOT_DEST(slot) ((SlotDest *)&(slot)->arg)

/* What an actor is doing, its substate (FIELDSTG_runActorAction) */
#define ACTOR_POSED 0 /* in the pose an event set (FIELDSTG_setActorPose) */
#define ACTOR_STAND 1
#define ACTOR_WALK 2
#define ACTOR_RUN 3
#define ACTOR_STOP 4 /* stops after a run */
#define ACTOR_WALK_OUT 5 /* walks off through an exit (FIELDSTG_walkActorInDir) */
#define ACTOR_CLIMB 0x40 /* holds on to a wall (FIELDSTG_controlClimb) */
#define ACTOR_CLIMB_UP 0x41
#define ACTOR_CLIMB_DOWN 0x42
#define ACTOR_GET_ON_WALL 0x43 /* from below (FIELDSTG_startClimbUp) */
#define ACTOR_GET_OVER_EDGE 0x44 /* onto the wall from above (FIELDSTG_startClimbDown) */
#define ACTOR_CLIMB_OFF_TOP 0x45
#define ACTOR_CLIMB_OFF_BOTTOM 0x46
#define ACTOR_DROP 0x47 /* FIELDSTG_startDrop */
#define ACTOR_GAUGE 0x48 /* plays the gauge game (FIELDSTG_startActorGauge) */
#define ACTOR_SEARCH 0x49 /* searches a hidden spot */
#define ACTOR_TALK 0x4A /* answers the actor that talks to it (talkPartner) */
#define ACTOR_FLY 0x4B /* flies on, triangle held (FIELDSTG_controlFlight) */
#define ACTOR_FLOAT 0x4C /* slows down in the air */
#define ACTOR_USE 0x4D /* works one of the objects 0x148, 0x15F and 0x160 */
#define ACTOR_USED 0x4E /* that object */
#define ACTOR_SLIDE 0x4F /* FIELDSTG_startActorSlide */
#define ACTOR_STOP_SLIDE 0x50

/*
 * A character on the field (FIELDSTG_createActor): the player (kind 0) and the
 * other characters. Registered with id FIELD_TASK_ACTOR, key1 = character, key2 = kind.
 * x and y are in 1/256 tile units.
 */
typedef struct Actor {
    TASK_HEADER(Actor);
    /* 0x050 */ Vec2 pos;
    /* 0x058 */ Vec2 tile;
    /* 0x060 */ s32 dir;
    /* 0x064 */ s32 z; /* how high it is off the ground, in 1/256 pixels (drawn that much higher) */
    /* 0x068 */ s32 speed; /* 0x400, 0x4CC in PAL's 50 Hz */
    /* 0x06C */ struct ActorImage *image;
    /* 0x070 */ struct FieldImage *fieldImage; /* the field's image, for the shadow */
    /* 0x074 */ s32 hasShadow; /* drawn with a shadow */
    /* 0x078 */ s32 depth; /* the layer's ordering table entry */
    /* 0x07C */ struct FieldActorEntry *entry; /* what created it, or NULL */
    /* 0x080 */ s32 halfWidth; /* half its width (FIELDSTG_actorWidths): its box's half width, and half that its half height */
    /* 0x084 */ s32 flying; /* the flying player (FIELDSTG_controlFlight) */
    /* 0x088 */ struct Actor *talkPartner; /* the actor that talks to it */
    /* 0x08C */ s32 climbSide; /* a climb shifts it a tile right (1) or left (0) */
    /* 0x090 */ s32 climbHeight; /* how high it has climbed, in 1/256 pixels (its shadow stays below) */
    /* 0x094 */ s32 wallHeight; /* the top of the climb */
    /* 0x098 */ s32 pad98; /* never read or written */
    /* 0x09C */ s32 animFile; /* its animations' file and index (FIELDSTG_fileEntries), or 0 */
    /* 0x0A0 */ s32 animSet; /* the animation set it plays (setAnim) */
    /* 0x0A4 */ s32 loadedSet; /* the set setAnims was loaded for */
    /* 0x0A8 */ s32 setAnims[5]; /* the set's animations, for the directions 0 to 4 */
    /* 0x0BC */ s32 walks; /* the pad walks it (ACTOR_WALK) instead of running */
    /* 0x0C0 */ s32 reloadImage; /* set to reload the frame's image */
    /* 0x0C4 */ s32 zSpeed; /* added to z each frame */
    /* 0x0C8 */ s16 voice; /* a sound voice, or -1 */
    /* 0x0CA */ s16 padCA; /* never read or written */
    /* 0x0CC */ s32 animPos; /* the next word of the direction's frames */
    /* 0x0D0 */ s32 animTime; /* the frame's time left */
    /* 0x0D4 */ s32 frame[4]; /* the frame's image, the one loaded, and two values */
    /* 0x0E4 */ s16 frameWidth; /* the loaded image's width in pixels */
    /* 0x0E6 */ s16 frameHeight; /* and its height */
    /* 0x0E8 */ s32 animDone; /* the animation ended (isAnimDone) */
    /* 0x0EC */ s32 walking; /* walking to the goal (setGoal, FIELDSTG_walkToGoal) */
    /* 0x0F0 */ s32 goalX; /* the tile it walks to */
    /* 0x0F4 */ s32 goalY;
    /* 0x0F8 */ s32 goalDir; /* the direction it then faces */
    /* 0x0FC */ u16 *talkActions; /* the talk's flag actions, applied when it ends, or NULL */
    /* 0x100 */ s32 keepsDir; /* doesn't turn to the one who talks to it (characters 0x28-0x2A,
                                   0x3E and 0x11A) */
    /* 0x104 */ struct Trail *trail; /* a follower's: the leader's steps */
    /* 0x108 */ void (*control)(struct Actor *); /* its update by kind, or NULL (resetControl) */
    /* 0x10C */ s32 scriptFlag; /* cleared by FIELDSTG_clearScriptFlag; nothing in FIELDSTG or the stages reads it */
    /* 0x110 */ void (*walkInDir)(struct Actor *, s32 dir);
    /* 0x114 */ void (*climbUp)();
    /* 0x118 */ void (*climbDown)();
    /* 0x11C */ void (*dropDown)();
    /* 0x120 */ void (*playGauge)();
    /* 0x124 */ void (*warp)(struct Actor *, SlotDest *dest, s32 kind);
    /* 0x128 */ void (*unused128)(); /* a method slot between warp and startWalk that
                                       FIELDSTG_createActor never sets and nothing calls */
    /* 0x12C */ void (*startWalk)(struct Actor *);
    /* 0x130 */ void (*resetControl)(struct Actor *);
    /* 0x134 */ void (*setDir)(struct Actor *, s32 dir);
    /* 0x138 */ s32 (*isAnimDone)(struct Actor *);
    /* 0x13C */ void (*setGoal)(struct Actor *, s32, s32, s32);
    /* 0x140 */ s32 (*isWalking)(struct Actor *);
    /* 0x144 */ void (*setAnim)(struct Actor *, s32);
    /* 0x148 */ void (*setPose)(struct Actor *, s32, s32 dir);
    /* 0x14C */ void (*getFacingTile)(struct Actor *, Vec2 *out);
    /* 0x150 */ void (*startSlide)(struct Actor *, s32 dir);
    /* 0x154 */ void (*stopSlide)(struct Actor *);
    /* 0x158 */ void (*launch)(struct Actor *, SlotDest *dest);
} Actor;

/* A battle of an area's BattleList, which can start on the field (see
   FIELDSTG_startEncounter); FIGHTSTG's Battle is the battle itself */
typedef struct AreaBattle {
    /* 0x0 */ s32 encounter; /* of FIELDSTG_encounters */
    /* 0x4 */ s32 stage; /* the fight stage, for BATTLE_SETUP.stage */
    /* 0x8 */ s32 music; /* for BATTLE_SETUP.music */
} AreaBattle;

/* The battles of an area of the map (its cells' value at FieldMap.files[4]),
   one picked at random */
typedef struct BattleList {
    /* 0x0 */ s32 count; /* how often they come: an index of FIELDSTG_battleRates */
    /* 0x4 */ AreaBattle *battles[8];
} BattleList;

/*
 * The battles of a place (FieldState.battles), by
 * area of the map: the fourth area's are also the ones that events start
 * (FIELDSTG_startEventBattle5, FIELDSTG_startEventBattle). A stage with several places has a list of
 * them, which FieldState.findBattles searches for the id.
 */
typedef struct FieldBattles {
    /* 0x00 */ s32 serial; /* a number of its own, 1 to 448 over the stages; nothing reads it */
    /* 0x04 */ s32 id; /* the place, GAME.place */
    /* 0x08 */ s32 pad8; /* 0 in every list; nothing reads it */
    /* 0x0C */ BattleList *battles[4];
} FieldBattles;

/*
 * The field's state (FIELDSTG_state): what a stage tells FIELDSTG about itself,
 * filled by its setup function (stageFuncs[0], FIELDSTG_setupField for the
 * field's own), then what FIELDSTG keeps of the field. The first 0x64 bytes
 * are cleared by FIELDSTG_pickStage, which also picks the stage overlay for
 * the current mode.
 *
 * The setup functions set start with a constructor, (Vec2){x, y}: GCC
 * clobbers the whole field before its two stores, which keeps the stores to
 * FIELDSTG_state on either side of it but lets the constants rise above it. The
 * match depends on that form: two stores of their own schedule otherwise.
 */
typedef struct FieldState {
    /* 0x00 */ s32 stageFile; /* the stage overlay's file */
    /* 0x04 */ struct Task *(*stageInit)(void *owner); /* starts the stage's task */
    /* 0x08 */ s32 mapFile; /* the file of the map's tiles (FIELDSTG_createMapStreamer) */
    /* 0x0C */ s32 sheetEntry; /* the stage's sprite sheet, loaded before the field starts, or 0 */
    /* 0x10 */ StageTile *objects; /* the map objects, up to the first margin 0 */
    /* 0x14 */ StageSlot *slots; /* up to the first type 0 */
    /* 0x18 */ s32 imageEntry; /* an image archive's file entry, loaded at (0x140, 0x100), or 0 */
    /* 0x1C */ s32 imageFile; /* or an image archive file, or 0 */
    /* 0x20 */ FieldBattles *battles;
    /* 0x24 */ FieldEvent *events; /* up to the first id -1 */
    /* 0x28 */ union {
        FieldImage *field; /* the field's image (the first two ActorImages) */
        ActorImage *actors; /* then the actors', from actors[2] */
    } images;
    /* 0x2C */ Vec2 start; /* where the player starts */
    /* 0x34 */ s32 startDir; /* and its direction */
    /* 0x38 */ CVECTOR spriteColor; /* the field's sprites' color, unless cd is 0 */
    /* 0x3C */ s32 soundBank; /* SOUND.loadBank's, or 0 */
    /* 0x40 */ s32 music; /* SOUND.playSound's, or 0 */
    /* 0x44 */ s32 textFile; /* the stage's text file, for the talks */
    /* 0x48 */ s32 eventText; /* the running event's text file and entry */
    /* 0x4C */ FieldActorEntry **actors; /* the characters, up to the first NULL */
    /* 0x50 */ s32 menuOpen; /* the field menu (START) or the inn (FIELDSTG_openInn) is open:
                               the pad doesn't move the player */
    /* 0x54 */ s32 frozen; /* no control, triggers or encounters for the player: while the
                             area name banner shows (FIELDSTG_createBanner) and as the field closes */
    /* 0x58 */ s32 busy; /* an event, a warp, a launch or a battle has the player */
    /* 0x5C */ s32 battleStarting; /* an encounter started (FIELDSTG_startEncounter) */
    /* 0x60 */ s32 acting; /* the player talks, climbs, slides or searches */
    /* 0x64 */ Vec2 defaultStart; /* where the player starts without a mode argument */
    /* 0x6C */ s32 defaultStartDir; /* and its direction */
    /* 0x70 */ void (*init)(void); /* FIELDSTG_pickStage */
    /* 0x74 */ s32 (*getFileEntry)(s32 character); /* FIELDSTG_fileEntries's */
    /* 0x78 */ s32 (*getActorWidth)(s32 character); /* FIELDSTG_actorWidths's */
    /* 0x7C */ FieldBattles *(*findBattles)(FieldBattles *list, s32 id); /* the place of the list with that id */
} FieldState;

extern FieldState FIELDSTG_state;

/* The children of a yes/no question at the bottom of the screen: FIELDSTG's
   ChoiceTask and the stages' copies of it (StageMenu) */
typedef struct ChoiceChildren {
    /* 0x00 */ TextWindow *title;
    /* 0x04 */ TextWindow *options[2];
    /* 0x0C */ Cursor *cursor;
    /* 0x10 */ struct EventTask *event; /* the answer's (FIELDSTG_startEvent) */
} ChoiceChildren;

/* FIELDSTG functions the stages call */
struct EventTask *FIELDSTG_startEvent(s32 id); /* creates the task of an event object */
StageTile *FIELDSTG_findObject(s32 anim); /* the first map object with that animation */
StageTile *FIELDSTG_findNextObject(void); /* and the next, or NULL */
void *FIELDSTG_startEventBattle5(void); /* starts a battle: the handler of the stages' events 9000 */

/* FIELDSTG's functions and data that the executable calls (game/events.c, system/overlay.c), by
   these names in its link (config/eu/undefined_syms.txt) */
void FIELDSTG_leaveField(s32 mode, s32 arg, s32 x, s32 y, s32 dir); /* leaves the field for a mode */
void FIELDSTG_startListedEvent(s32 index); /* starts the event FIELDSTG_eventIds[index] */
void FIELDSTG_openInn(void); /* opens the inn */
typedef struct FieldBattleFuncs {
    void (*startEventBattle)(s32 index); /* FIELDSTG_startEventBattle */
    void (*startAreaBattle)(void); /* FIELDSTG_startAreaBattle */
} FieldBattleFuncs;
extern FieldBattleFuncs FIELDSTG_battleFuncs;

#endif
