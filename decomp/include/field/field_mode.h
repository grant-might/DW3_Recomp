#ifndef FIELDSTG_H
#define FIELDSTG_H

/*
 * FIELDSTG.PRO: the field mode, where the player walks around the map.
 *
 * The stage overlays (AAA/PRO/WSTAG###.PRO, see stage.h) load on top of it
 * at 0x800A4CA4 and call into it; what they and the executable use of it is
 * in field_map.h. This header has the rest: the types, by the module
 * (in src/field/field_mode) that owns them, in an order that defines each before
 * its use; the functions, by module in the order they link; and the data
 * (data/fieldstg.c) in its order.
 */

#include "engine/game.h"
#include "field/field_map.h"

/* Names the functions of the src/shared files it shares */
#define OVL_NAME(name) FIELDSTG_##name

/* --- Shared: the types several modules use --- */

/* Where the field keeps its textures in VRAM: the two images of
   FIELD_SPRITES_FILE (its entries 2 and 3, for the sprite sheets of its
   entries 0 and 1), the stage's map objects (FIELDSTG_loadFieldFiles) and a
   cutscene's frames (FIELDSTG_playCutsceneAnim) */
#define FIELD_SPRITES_X 0x200
#define FIELD_SPRITES2_X 0x240
#define FIELD_SPRITES_Y 0x100
#define FIELD_OBJECTS_X 0x140
#define FIELD_OBJECTS_Y 0x100
#define FIELD_OBJECTS_CLUT_Y 0x1F0
#define FIELD_CUTSCENE_X 0x280
#define FIELD_CUTSCENE_CLUT_Y 0xF0
#define FIELD_MENU_SPRITES_X 0x140 /* the executable's FILE_MENU_SPRITES */

/*
 * The sounds only FIELDSTG plays (SOUND.playSound), by their SOUNDTST names.
 * Those that other overlays play too are in engine/sound.h.
 */
#define SOUND_DIG_DEMO 0x40003
#define SOUND_DIGIMENT 0x40004 /* a warp's effect */
#define SOUND_ENCOUNTS 0x40005 /* the transition to a battle */
#define SOUND_FUKIDASH 0x40007 /* a balloon */
#define SOUND_ITEM_GET 0x40009
#define SOUND_PIYOPIYO 0x40013
#define SOUND_SAVEDEMO 0x40015
#define SOUND_SUB_DEMO 0x40018
#define SOUND_BULB_003 0x440001
#define SOUND_TRAP_OFF 0x700001
#define SOUND_INFO_SIG 0xB80001
#define SOUND_BM_ERASE 0x1100000
#define SOUND_LD_ERASE 0x1100002
#define SOUND_DEMO_BGM 0x60040002
#define SOUND_COMCD111 0x80042DC7
#define SOUND_COMCD201 0x800430BD
#define SOUND_PLAYER00 0x8004583C
#define SOUND_PLAYER01 0x800458BD
#define SOUND_PLAYER02 0x8004593E
#define SOUND_PLAYER08 0x80045C44
#define SOUND_PLAYER09 0x80045CC5
#define SOUND_PLAYER10 0x80045D46
#define SOUND_TRESUREB 0x80045DC7
#define SOUND_DIG_MOVE 0x80045FCB
#define SOUND_MASK_SET 0x803C503C
#define SOUND_BEAM_SHT 0x805458BD
#define SOUND_WEAR_OFF 0x80E8383C
#define SOUND_SN_ENTRY 0x8110303C
#define SOUND_SN_ERASE 0x81103240
#define SOUND_COMCD203 0xA00431BF /* held while the camera shakes */
#define SOUND_SUB_MOVE 0xA0045F4A /* held */
#define SOUND_BEAM_HIT 0xA054583C /* held */
#define SOUND_TRAP_ICE 0xA064683C /* held */
#define SOUND_GAYALOOP 0xA10C703C /* held */

/* --- actor.c --- */

/* The animation sets that the actions play (FIELDSTG_setActorAnim), by the
   substate that plays them */
#define ACTOR_ANIM_STAND 1
#define ACTOR_ANIM_WALK 4
#define ACTOR_ANIM_RUN 5
#define ACTOR_ANIM_STOP 6
#define ACTOR_ANIM_SEARCH 8
#define ACTOR_ANIM_GAUGE_START 0x11
#define ACTOR_ANIM_GAUGE_RESULT 0x12 /* after the game, with a balloon */
#define ACTOR_ANIM_GAUGE_PLAY 0x13 /* with PLAYER09 */
#define ACTOR_ANIM_GAUGE_WAIT 0x14 /* with PLAYER10, until the game ends */
#define ACTOR_ANIM_GAUGE_END 0x15
#define ACTOR_ANIM_DROP_OFF 0x16
#define ACTOR_ANIM_FALL 0x17
#define ACTOR_ANIM_FALL_FAR 0x18 /* once the fall is more than 0x1800 */
#define ACTOR_ANIM_LAND 0x19
#define ACTOR_ANIM_CLIMB_UP 0x1A
#define ACTOR_ANIM_CLIMB_DOWN 0x1B
#define ACTOR_ANIM_GET_ON_WALL 0x1C
#define ACTOR_ANIM_CLIMB_OFF_TOP 0x1D
#define ACTOR_ANIM_GET_OVER_EDGE 0x1E
#define ACTOR_ANIM_CLIMB_OFF_BOTTOM 0x1F
#define ACTOR_ANIM_CLIMB 0x20
#define ACTOR_ANIM_OPEN 0x41 /* a treasure's, when it is talked to */
#define ACTOR_ANIM_USE 0x45
#define ACTOR_ANIM_USED 0x54 /* the object's */

/*
 * Flight (FIELDSTG_controlFlight, FIELDSTG_checkFlightProbe): an actor's z
 * against the floor map's cells. Cell 1 is open; a wall, cell n from 2 to
 * 6, stands up to FLIGHT_LEVEL(n); a ceiling, cell 24 - n from 18 to 22,
 * hangs from FLIGHT_LEVEL(n) up.
 */
#define FLIGHT_OPEN 1
#define FLIGHT_WALL(n) (n)
#define FLIGHT_CEILING(n) (24 - (n))
#define FLIGHT_LEVEL(n) ((n) << 12) /* n * 16 pixels, in z's 1/256 pixels */
#define FLIGHT_LANDING 0xC /* how far above a wall's top a descent stops */
#define FLIGHT_Z_MIN 0x1800
#define FLIGHT_Z_MAX 0x7000
#define FLIGHT_SPEED_MAX 0x200
#define FLIGHT_SPEED_MAX_PAL 0x266 /* FLIGHT_SPEED_MAX * 6 / 5, for 50 frames a second */

/* Actor and its substates (ACTOR_POSED...) are in field_map.h, where the stages see them too */

/* The last TRAIL_STEPS steps of the actor that another one follows
   (Actor.trail), a ring from tail to head */
#define TRAIL_STEPS 64 /* a power of two */

typedef struct TrailStep {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 dir;
} TrailStep;

typedef struct Trail {
    /* 0x000 */ Actor *leader;
    /* 0x004 */ s32 head;
    /* 0x008 */ s32 tail;
    /* 0x00C */ TrailStep steps[TRAIL_STEPS];
} Trail;

/* The children of an actor (FIELDSTG_updateActor) */
typedef struct ActorChildren {
    /* 0x0 */ struct ActorIcon *icon; /* FIELDSTG_createActorIcon's */
    /* 0x4 */ struct Balloon *balloon; /* FIELDSTG_createBalloon's */
    /* 0x8 */ void *action; /* a gauge game or a launch */
    /* 0xC */ struct Speech *speech; /* FIELDSTG_createTalk's */
} ActorChildren;

/* --- actor_icon.c --- */

/* An offset of the icon from the player's tile along a climb over an edge
   (FIELDSTG_iconPath), x away from the climb's side */
typedef struct IconOffset {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
} IconOffset;

/* The player's icon (FIELD_TASK_ICON, FIELDSTG_createActorIcon): the field
   commands FIELD_COMMAND_ICON1 to 3 and a special condition set its
   substate, the animation it plays */
typedef struct ActorIcon {
    TASK_HEADER(ActorIcon);
    /* 0x50 */ Actor *actor;
    /* 0x54 */ s32 frame;
    /* 0x58 */ s32 animStep;
    /* 0x5C */ s32 animTime;
    /* 0x60 */ u8 (*anim)[2]; /* (frame, time) pairs; 0xFF loops to a step */
} ActorIcon;

/* --- balloon.c --- */

/* A balloon over an actor's head (FIELDSTG_createBalloon): it pops up, plays its
   FIELDSTG_triggerAnims row (key2) and pops down when closed */
typedef struct Balloon {
    TASK_HEADER(Balloon);
    /* 0x50 */ Actor *actor; /* the player when NULL */
    /* 0x54 */ s32 popStart; /* the pop-up's sprites, times 4: it opens from here */
    /* 0x58 */ s32 popOpen; /* to here */
    /* 0x5C */ s32 popEnd; /* and closes to here */
    /* 0x60 */ s32 pop; /* the pop-up's sprite, times 4 */
    /* 0x64 */ s32 frame; /* into its FIELDSTG_triggerAnims row */
    /* 0x68 */ s32 time;
} Balloon;

/* A character that another answers for when the player talks to it
   (FIELDSTG_talkToActorAt, FIELDSTG_standIns) */
typedef struct StandIn {
    /* 0x0 */ s16 character; /* Actor.key1 */
    /* 0x2 */ s16 answerer; /* the key1 of the actor that answers */
} StandIn;

/* --- speech.c --- */

/* A message box, or a talk box that follows an actor (FIELDSTG_createSpeech,
   FIELDSTG_createTalk) */
typedef struct Speech {
    TASK_HEADER(Speech);
    /* 0x50 */ Actor *actor;
    /* 0x54 */ s32 entry; /* the text's entry */
    /* 0x58 */ s32 type; /* the talk box's (createTalkBox), its corner */
    /* 0x5C */ s32 isMessage; /* a message box */
    /* 0x60 */ void *text; /* FILE_CACHE.getEntry's */
} Speech;

/* --- launch.c --- */

/* Launches an actor, spinning, from the nearest launcher (FIELD_TASK_LAUNCHER)
   to a tile (FIELDSTG_createLaunch, Actor.launch) */
typedef struct Launch {
    TASK_HEADER(Launch);
    /* 0x50 */ Actor *actor;
    /* 0x54 */ SlotDest *dest; /* the tile it lands on */
    /* 0x58 */ Task *from; /* the nearest FIELD_TASK_LAUNCHER */
    /* 0x5C */ Vec2 start;
    /* 0x64 */ Vec2 dist;
    /* 0x6C */ s32 negX;
    /* 0x70 */ s32 negY;
} Launch;

/* --- gauge.c --- */

/* A gauge game (FIELDSTG_createGauge, FIELDSTG_runGauge) */
#define GAUGE_ROWS 8 /* the random rows of FIELDSTG_gaugeRows */
#define GAUGE_EMPTY_ROW 8 /* the European version's, all zeros */
#define GAUGE_CURSOR_MAX 0x3000 /* the row's 48 pixels, in 1/256 pixels */
#define GAUGE_START_DELAY 0x5A /* frames before the cursor runs */
#define GAUGE_RESULT_DELAY 0x3C /* frames on the stopped cursor */
#define GAUGE_X 0x18 /* where it is drawn, on the screen */
#define GAUGE_Y 0xC0
#define GAUGE_FRAME 0x3C /* its sprites, of FIELD_SPRITES_FILE's sheet 1 */
#define GAUGE_CURSOR_FRAME 0x3D
#define GAUGE_ROW_FRAMES 0x3E /* + the row */

/* The steps of a running gauge */
#define GAUGE_RUNNING 0 /* until cross is pressed */
#define GAUGE_STOPPING 1
#define GAUGE_STOPPING_SLOWLY 2 /* one time in four */
#define GAUGE_STOPPED 3

typedef struct GaugeGame {
    TASK_HEADER(GaugeGame);
    /* 0x50 */ Vec2 pos;
    /* 0x58 */ s32 row; /* of FIELDSTG_gaugeRows */
    /* 0x5C */ s32 cursor; /* along the row, 0-GAUGE_CURSOR_MAX */
    /* 0x60 */ s32 speed;
    /* 0x64 */ s32 back; /* the cursor goes back */
} GaugeGame;

/* --- camera.c --- */

/* The camera (FIELD_TASK_CAMERA, FIELDSTG_createCamera): it centers the field's
   layer on an actor or a spot, panning there unless it snaps, and shakes */
typedef struct Camera {
    TASK_HEADER(Camera);
    /* 0x50 */ Actor *target; /* the actor it follows (substate 0) */
    /* 0x54 */ Vec2 center; /* where it looks */
    /* 0x5C */ s32 shaking;
    /* 0x60 */ s32 shake; /* the step of the shake, 0-3 */
    /* 0x64 */ s16 voice; /* of the shaking sound, or -1 */
    /* 0x68 */ Vec2 pan; /* where it looks while it pans to center */
    /* 0x70 */ s32 hasBounds;
    /* 0x74 */ Vec2 bounds; /* the map's size */
    /* 0x7C */ s32 unk7C; /* cleared with each new target; nothing reads it */
    /* 0x80 */ s32 snap; /* goes to center at once instead of panning */
    /* 0x84 */ s32 targetId; /* the target's character (Actor.key1) */
    /* 0x88 */ s32 spotX; /* the spot it looks at (substate 1) */
    /* 0x8C */ s32 spotY;
} Camera;

/* --- event.c --- */

/*
 * An event (FIELDSTG_runEvent, made by FIELDSTG_startEvent): it runs a script
 * of 16-bit words, each command a word of its kind << 8 | its variant followed
 * by its arguments, until a command waits; or, without a script, a task of
 * its own (start) until it ends.
 */
typedef struct EventTask {
    TASK_HEADER(EventTask);
    /* 0x050 */ s32 event; /* its id */
    /* 0x054 */ s16 *pc;
    /* 0x058 */ void *(*start)(void);
    /* 0x05C */ void (*end)(void);
    /* 0x060 */ s32 wait; /* frames left of a wait command */
    /* 0x064 */ struct {
        s32 id; /* 0 ends the list */
        struct Actor *actor;
    } entries[30]; /* the characters, held while the event runs */
} EventTask;

/* The children of an EventTask */
typedef struct EventChildren {
    /* 0x00 */ struct Task *task; /* start's */
    /* 0x04 */ struct Speech *boxes[3]; /* the script's message boxes */
    /* 0x10 */ s32 scripts[10]; /* FIELDSTG_createScriptCommand's tasks */
} EventChildren;

/* --- script.c --- */

/* An entry of the script command table FIELDSTG_scriptCommands (ids from 0x320) */
typedef struct ScriptCommand {
    /* 0x0 */ s32 id;
    /* 0x4 */ s32 (*create)(s32 id); /* creates the command's task */
    /* 0x8 */ void (*handle)(); /* (task, command, arg): each command's task has its own type */
} ScriptCommand;

/* A wait timer for the scripts (FIELDSTG_waitScriptTime) */
typedef struct ScriptTimer {
    /* 0x0 */ s32 time;
    /* 0x4 */ s32 active;
    /* 0x8 */ void (*reset)(void);
    /* 0xC */ Actor *(*findActor)(s32 character);
} ScriptTimer;

/* --- lift.c --- */

/* Lift is in field_map.h: WSTAG261 and WSTAG934 have a copy of the lift */

/* --- story.c --- */

/* The story events of the field (FIELDSTG_createStoryEvents) */
typedef struct StoryEvents {
    TASK_HEADER(StoryEvents);
    /* 0x50 */ void *owner; /* the field's task, as a stage's init gets it */
    /* 0x54 */ s32 script; /* the script to run next, or 0 */
} StoryEvents;

typedef struct StoryEventsChildren {
    /* 0x0 */ Lift *lift;
    /* 0x4 */ EventTask *script;
} StoryEventsChildren;

/* A story event (FIELDSTG_progressEvents): at a progress, while a flag is clear and a
   condition holds, a script runs, and then another one */
typedef struct ProgressEvent {
    /* 0x0 */ s32 progress; /* -1 ends the list */
    /* 0x4 */ s32 flag;
    /* 0x8 */ s32 condition;
    /* 0xC */ s16 script;
    /* 0xE */ s16 nextScript;
} ProgressEvent;

/* --- choice.c --- */

/* A yes/no question of the story (FIELDSTG_runChoice); its children are a
   ChoiceChildren (field_map.h) */
typedef struct ChoiceTask {
    TASK_HEADER(ChoiceTask);
    /* 0x50 */ s32 type; /* FIELDSTG_choices's */
    /* 0x54 */ s32 selection;
    /* 0x58 */ PanelAnim tween; /* the panel's width */
} ChoiceTask;

/* A question of a ChoiceTask */
typedef struct ChoiceText {
    /* 0x0 */ s32 text; /* the file counted from TEXT_FILE(1) << 16 | its
                           entry: the question, then the answers */
    /* 0x4 */ s16 events[2]; /* FIELDSTG_startEvent's, for each answer */
} ChoiceText;

/* --- cutscene.c --- */

/* A cutscene's animation over the field (FIELDSTG_createCutsceneAnim) */
typedef struct CutsceneAnim {
    TASK_HEADER(CutsceneAnim);
    /* 0x50 */ s32 kind; /* 0: file 0x88C (0x87B in the USA), 1: file 0x88D (0x87C) */
    /* 0x54 */ s32 frame;
    /* 0x58 */ s32 index; /* into the animation */
    /* 0x5C */ s32 timer;
    /* 0x60 */ s32 frame2; /* kind 1's second animation */
    /* 0x64 */ s32 index2;
    /* 0x68 */ s32 timer2;
} CutsceneAnim;

/* The two animations of FIELDSTG_playCutsceneAnim, kind 0's file and kind 1's after it:
   the discs number their files differently */
#if VERSION_US || VERSION_JP
#define FIELD_ANIM_FILE 0x87B
#elif VERSION_EU
#define FIELD_ANIM_FILE 0x88C
#endif

/* --- effect.c --- */

/* An animation of an FieldEffect (FIELDSTG_stepEffectAnim) */
typedef struct EffectAnim {
    /* 0x0 */ s32 active;
    /* 0x4 */ AnimState anim;
} EffectAnim;

/* An effect of up to four animations at a spot (FIELDSTG_createEffect) */
typedef struct FieldEffect {
    TASK_HEADER(FieldEffect);
    /* 0x50 */ s32 x;
    /* 0x54 */ s32 y;
    /* 0x58 */ s16 set; /* the row of FIELDSTG_effectAnims */
    /* 0x5C */ EffectAnim anims[4];
} FieldEffect;

/* --- map.c --- */

/* A rectangle, edges included */
typedef struct Box {
    /* 0x0 */ s32 left;
    /* 0x4 */ s32 right;
    /* 0x8 */ s32 top;
    /* 0xC */ s32 bottom;
} Box;

/* --- stream.c --- */

/* A sprite of a StreamTask's frame */
typedef struct StreamSprite {
    /* 0x00 */ s32 visible;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 u;
    /* 0x10 */ s32 v;
    /* 0x14 */ s32 w;
    /* 0x18 */ s32 h;
} StreamSprite;

/* A task that reads the frames of a file from the disc (FIELDSTG_createStream) */
typedef struct StreamTask {
    TASK_HEADER(StreamTask);
    /* 0x050 */ s32 time;
    /* 0x054 */ s32 loadedTime; /* when its read ended, 0 while reading */
    /* 0x058 */ s32 frame;
    /* 0x05C */ s32 sector;
    /* 0x060 */ s32 file;
    /* 0x064 */ s32 frameSectors;
    /* 0x068 */ void *buffer;
    /* 0x06C */ s32 loaded;
    /* 0x070 */ s32 loadedSlot; /* the slot its image is loaded into, or -1 */
    /* 0x074 */ s32 imageX;
    /* 0x078 */ s32 imageY;
    /* 0x07C */ s32 clutX;
    /* 0x080 */ s32 clutY;
    /* 0x084 */ StreamSprite sprites[3][5];
    /* 0x228 */ s32 slot;
    /* 0x22C */ Decompressor *source; /* what it reads its sprites from (FIELDSTG_setStreamSource) */
    /* 0x230 */ s32 *data; /* the decompressed tile: its sprites, then its image */
    /* 0x234 */ void (*seek)(struct StreamTask *task, s32 frame, s32 size);
    /* 0x238 */ s32 (*isLoaded)(struct StreamTask *task);
    /* 0x23C */ void (*draw)(struct StreamTask *task, Layer *layer, s32 x, s32 y);
    /* 0x240 */ void (*setSource)(struct StreamTask *task, s32 slot, Decompressor *source);
    /* 0x244 */ s32 (*getFrame)(struct StreamTask *task);
    /* 0x248 */ s32 (*getSlot)(struct StreamTask *task);
    /* 0x24C */ void (*clearSlot)(struct StreamTask *task);
    /* 0x250 */ void (*updateTime)(struct StreamTask *task);
} StreamTask;

/* The StreamTasks of a task's children (FIELDSTG_requestTiles) */
typedef struct StreamPool {
    /* 0x00 */ Decompressor *decompressor;
    /* 0x04 */ StreamTask *tasks[30];
} StreamPool;

/* --- map_streamer.c --- */

/* The first sector of a map's tile file (MapStreamer.header) */
typedef struct MapFileHeader {
    /* 0x00 */ s32 tileCount; /* the tiles with data (tileSizes not 0); nothing reads it */
    /* 0x04 */ s32 width; /* in 128-pixel tiles */
    /* 0x08 */ s32 height;
    /* 0x0C */ s32 tileBytes; /* the room of a tile on the CD */
    /* 0x10 */ u16 tileSizes[FLEXIBLE]; /* width * height: each tile's bytes, 0 for none */
} MapFileHeader;

/* A tile of the map that MapStreamer streams */
typedef struct MapTile {
    /* 0x0 */ s32 size; /* its bytes on the CD, 0 for none */
    /* 0x4 */ s32 sectors; /* what it reads (StreamTask.seek) */
} MapTile;

/* One of the 12 VRAM slots the map's tiles are loaded into (FIELDSTG_slotImages) */
typedef struct MapImage {
    /* 0x0 */ s32 frame; /* the tile it holds, or -1 */
    /* 0x4 */ s32 time; /* when it was last drawn */
    /* 0x8 */ s32 cover; /* the level of the cover over a new tile (8.8), 0 for none */
} MapImage;

/*
 * The task that streams the map's tiles from the CD around the view
 * (FIELDSTG_runMapStreamer, id 4, created by FIELDSTG_createMapStreamer)
 */
typedef struct MapStreamer {
    TASK_HEADER(MapStreamer);
    /* 0x050 */ s32 pad50; /* never read or written */
    /* 0x054 */ MapFileHeader *header; /* the map file's, while it is read */
    /* 0x058 */ Vec2 scroll;
    /* 0x060 */ s32 pad60; /* never read or written */
    /* 0x064 */ s32 file; /* the map's tiles, read from the CD (FieldState.mapFile) */
    /* 0x068 */ s32 width; /* in 128-pixel tiles */
    /* 0x06C */ s32 height;
    /* 0x070 */ s32 frameSectors; /* a tile's sectors on the CD */
    /* 0x074 */ MapImage images[12];
    /* 0x104 */ MapTile *tiles; /* width * height */
    /* 0x108 */ u8 viewTiles[30]; /* by the slots around the view, their tiles */
    /* 0x128 */ s32 viewX; /* the tile column of the slots' left edge */
    /* 0x12C */ s32 viewY; /* their top row */
    /* 0x130 */ Vec2 *(*getSize)(struct MapStreamer *); /* in pixels (FIELDSTG_getMapSize) */
} MapStreamer;

/* --- map_objects.c --- */

/* Animates and draws the map's objects (FIELDSTG_createMapObjects) */
typedef struct MapObjects {
    TASK_HEADER(MapObjects);
    /* 0x50 */ s32 sprites; /* their sprite file and entry */
    /* 0x54 */ struct StageTile *objects;
} MapObjects;

/* --- trigger.c --- */

/* The map's triggers (FIELDSTG_createTriggers): on a trigger's cell (layer
   7: index and direction) the player sets it off at once or, with a balloon,
   on the action button */
typedef struct Triggers {
    TASK_HEADER(Triggers);
    /* 0x50 */ s32 sheetEntry; /* the stage's sprite sheet (FieldState.sheetEntry); nothing reads it */
    /* 0x54 */ StageSlot *entries; /* FieldState.slots */
    /* 0x58 */ Actor *actor; /* the player */
    /* 0x5C */ s32 index; /* the trigger under it */
    /* 0x60 */ s32 dir; /* the trigger's direction */
    /* 0x64 */ StageSlot *entry;
} Triggers;

/* The children of an Triggers */
typedef struct TriggerChildren {
    /* 0x0 */ struct Balloon *balloon;
    /* 0x4 */ EventTask *script;
} TriggerChildren;

/* --- hidden_spots.c --- */

/* The hint of a hidden spot that held nothing (FIELDSTG_createSpotHint): its sprite
   and its palette cycle tell how far the prize is */
typedef struct SpotHint {
    TASK_HEADER(SpotHint);
    /* 0x50 */ s32 time;
    /* 0x54 */ s32 speed; /* frames per palette step */
    /* 0x58 */ s32 frame;
    /* 0x5C */ Vec2 from;
    /* 0x64 */ Vec2 to;
} SpotHint;

/* A hidden spot: a map object with anim 0xFF */
typedef struct HiddenSpot {
    /* 0x00 */ s32 frame; /* its object's */
    /* 0x04 */ s32 object; /* its index in FieldState.objects */
    /* 0x08 */ Vec2 pos;
    /* 0x10 */ s32 hasPrize; /* the one picked at random (GAME.prizeSpot) */
} HiddenSpot;

/* The map's hidden spots (FIELD_TASK_HIDDEN_SPOTS, FIELDSTG_createHiddenSpots):
   one of them, picked at random, holds the prize */
typedef struct HiddenSpots {
    TASK_HEADER(HiddenSpots);
    /* 0x50 */ s32 count;
    /* 0x54 */ HiddenSpot *entries;
    /* 0x58 */ s32 selected; /* the spot searched */
    /* 0x5C */ s32 forEvent; /* an event's search, without the prize or a hint */
    /* 0x60 */ Vec2 pos; /* the prize's */
} HiddenSpots;

/* The children of an HiddenSpots */
typedef struct HiddenSpotsChildren {
    /* 0x0 */ struct SpotEffect *effect;
    /* 0x4 */ struct SpotHint *hint;
} HiddenSpotsChildren;

/* The effect of searching a hidden spot, in front of the actor that searches
   (FIELDSTG_createSpotEffect) */
typedef struct SpotEffect {
    TASK_HEADER(SpotEffect);
    /* 0x50 */ s32 x;
    /* 0x54 */ s32 y;
    /* 0x58 */ s32 dir;
    /* 0x5C */ s32 frame;
    /* 0x60 */ u8 *anim;
} SpotEffect;

/* --- file_loader.c --- */

/* The field's effect sprites (FIELDSTG_drawSpotEffect): the discs number their files
   differently */
#if VERSION_US || VERSION_JP
#define FIELD_SPRITES_FILE 0x152
#elif VERSION_EU
#define FIELD_SPRITES_FILE 0x160
#endif

/* The files of the player's actions, which FIELDSTG_requestSlotFiles loads
   when the map has slots for them: the discs number their files differently */
#if VERSION_US || VERSION_JP
#define FIELD_ACTION_FILES 0x3B9
#elif VERSION_EU
#define FIELD_ACTION_FILES 0x3C9
#endif
#define FIELD_DROP_FILE FIELD_ACTION_FILES
#define FIELD_GAUGE_FILE (FIELD_ACTION_FILES + 1)
#define FIELD_SEARCH_FILE (FIELD_ACTION_FILES + 2) /* the hidden spots' */
#define FIELD_CLIMB_FILE (FIELD_ACTION_FILES + 3)

/* --- banner.c --- */

/* The name of a mode's area (FIELDSTG_showAreaName), up to the first mode 0: the
   strings of the two windows, from text files 0xAA and 0xB8 */
typedef struct AreaName {
    /* 0x0 */ u8 area;
    /* 0x1 */ u8 place;
    /* 0x2 */ s16 mode;
} AreaName;

/* The windows of FIELDSTG_showAreaName */
typedef struct AreaNameWindows {
    /* 0x0 */ TextWindow *area;
    /* 0x4 */ TextWindow *place;
} AreaNameWindows;

/* A rectangle of an AreaBanner, which can stretch to a new range */
typedef struct BannerBox {
    /* 0x00 */ s32 visible;
    /* 0x04 */ DVECTOR pos;
    /* 0x08 */ DVECTOR size;
    /* 0x0C */ s32 color;
    /* 0x10 */ s32 stretch; /* 1: horizontally, 2: vertically */
    /* 0x14 */ s32 from;
    /* 0x18 */ s32 to;
    /* 0x1C */ s32 speed;
    /* 0x20 */ s32 pad20; /* 0 in every box; nothing reads it */
} BannerBox;

/* The area name banner (FIELD_TASK_BANNER, FIELDSTG_createBanner) */
typedef struct AreaBanner {
    TASK_HEADER(AreaBanner);
    /* 0x050 */ BannerBox boxes[10];
    /* 0x1B8 */ RECT clip; /* the layer's, closing in TASK_DONE */
} AreaBanner;

/* --- state.c --- */

/* An entry of the stage tables (FIELDSTG_pickStage): the stage overlay of a mode,
   up to the first mode 0 */
typedef struct StageEntry {
    /* 0x0 */ s32 mode;
    /* 0x4 */ s32 file;
    /* 0x8 */ struct Task *(*init)(void *owner);
} StageEntry;

/* --- field.c --- */

/* The player's FLAGS_00 codes (FIELDSTG_controlPlayer) */
#define FIELD_FLAG_TALK_AHEAD 0x12 /* the player talks to what it faces, and clears it */
#define FIELD_SEARCH_ITEM ITEM(0, 4) /* item 4, which the player needs to search the hidden spots */

/* The flag set when the field of a mode is entered: FLAGS_00's group 0x20,
   flag mode - 0x200 */
#define FIELD_VISITED_FLAG(mode) (0x2000 + (mode) - 0x200)

/* The modes of two fields that keep the file cache (FIELDSTG_keepsFileCache) */
#define FIELD_MODE_WSTAG415 0x22D /* where the player's run doesn't check the probes
                                       ahead (FIELDSTG_checkFlightProbes) */
#define FIELD_MODE_WSTAG815 0x2DE

/* The fighters whose battles always give an item (FIELDSTG_startEncounter) */
#define FIELD_PRIZE_FIGHTERS 0x1C9
#define FIELD_PRIZE_FIGHTER_COUNT 8

/* FIELDSTG_playBattleTransition's tiles: a copy of the screen at
   TRANSITION_IMAGE_X in VRAM, cut into columns and rows */
#define TRANSITION_COLUMNS 5
#define TRANSITION_ROWS 6
#define TRANSITION_TILE_WIDTH (SCREEN_WIDTH / TRANSITION_COLUMNS)
#define TRANSITION_TILE_HEIGHT (SCREEN_HEIGHT / TRANSITION_ROWS)
#define TRANSITION_IMAGE_X 0x280
#define TRANSITION_SPEED 40 /* pixels a frame */
#define TRANSITION_GONE 0x200 /* a tile's coordinate once it has slid off */
#define TRANSITION_WAIT 0x10 /* the steps after the last tile, before the mode */
#define TRANSITION_DONE 0x1000 /* the step once the mode is requested */

/* A step of FIELDSTG_playBattleTransition's spiral: one coordinate of a tile moves by dir
 * until it passes limit */
typedef struct TileMove {
    /* 0x0 */ s32 *value;
    /* 0x4 */ s32 dir; /* 1, -1, or 0 at the end */
    /* 0x8 */ s32 limit;
} TileMove;

/* The field's own image (FieldState.images) and its actors' after it */
typedef struct FieldImages {
    /* 0x00 */ FieldImage field;
    /* 0x20 */ ActorImage actors[20];
} FieldImages;

/* The field's main task (FIELDSTG_updateField, FIELD_TASK_FIELD); its children follow */
typedef struct FieldTask {
    TASK_HEADER(FieldTask);
    /* 0x50 */ s32 fade; /* FIELDSTG_drawCover's level */
    /* 0x54 */ s32 width; /* of the clip rectangle */
    /* 0x58 */ s32 height;
    /* 0x5C */ s32 nextMode; /* the mode it leaves for (FIELDSTG_leaveFieldAfter) */
    /* 0x60 */ s32 nextModeArg;
    /* 0x64 */ void *highBuffer; /* a buffer at the end of the heap */
    /* 0x68 */ s32 leaveDelay; /* the frames left before it closes the field */
    /* 0x6C */ s32 centerOnPlayer; /* the closing clip shrinks onto the player */
    /* 0x70 */ s32 warpKind; /* the effect and cutscene of a warp (FIELDSTG_startWarp) */
    /* 0x74 */ Vec2 warpPos; /* where the warp's effect plays */
    /* 0x7C */ SlotDest *warp; /* where it leads */
} FieldTask;

/* The children of the field's main task (FieldTask, FIELDSTG_updateField) */
typedef struct FieldChildren {
    /* 0x00 */ struct FieldEffect *effect; /* FIELDSTG_createEffect's */
    /* 0x04 */ ScreenFade *fade;
    /* 0x08 */ struct CutsceneAnim *cutscene; /* FIELDSTG_createCutsceneAnim's */
    /* 0x0C */ EventTask *event;
    /* 0x10 */ union {
        Task *fileLoader; /* FIELDSTG_createFileLoader's, as the field starts */
        struct Inn *inn; /* createInn's (FIELDSTG_openInn) */
    } awaited; /* the task the field waits for, until it ends */
    /* 0x14 */ Task *banner; /* FIELDSTG_createBanner's */
    /* 0x18 */ struct Actor *actors[4]; /* the player and the partners */
    /* 0x28 */ struct Camera *camera;
    /* 0x2C */ struct Actor *npcs[15]; /* FieldState.actors's other characters */
    /* 0x68 */ struct Triggers *triggers; /* FIELDSTG_createTriggers's */
    /* 0x6C */ Task *stage; /* FieldState.stageInit's */
    /* 0x70 */ struct MapStreamer *mapStreamer; /* FIELDSTG_createMapStreamer's */
    /* 0x74 */ FieldMenu *menu; /* createFieldMenu's */
    /* 0x78 */ struct MapObjects *mapObjects; /* FIELDSTG_createMapObjects's */
} FieldChildren;

/* An encounter of FIELDSTG_encounters, which FIELDSTG_startEncounter starts: its enemies and
   the bytes it copies to BATTLE_SETUP.ambushChance on */
typedef struct Encounter {
    /* 0x00 */ BattleEnemy *enemies[3];
    /* 0x0C */ u8 ambushChance;
    /* 0x0D */ u8 kind; /* copied to BATTLE_SETUP.encounterKind: 1 for nearly all the maps'
                          random battles (some enemies' battle table actions, condition 13, run
                          away in them), 4 for the Digimon Arena's, 2, 3 and 5 mostly for the
                          event battles */
    /* 0x0E */ u8 blocks[12]; /* copied to BATTLE_SETUP.blocks */
} Encounter;

/* The functions, by module in the order they link */

/* commands.c */
void FIELDSTG_updateCommandTask(Task *task);
void FIELDSTG_handleFieldCommand(Task *task, s32 command);
void FIELDSTG_startCommandTask(void);

/* lift.c */
void FIELDSTG_updateLift(Lift *task);
void FIELDSTG_moveLift(Lift *task, s32 command);
Lift *FIELDSTG_createLift(s32 id);

/* choice.c */
void FIELDSTG_runChoice(ChoiceTask *task, ChoiceChildren *children);
void *FIELDSTG_askChoice0(void), *FIELDSTG_askChoice1(void), *FIELDSTG_askChoice2(void), *FIELDSTG_askChoice3(void);
void *FIELDSTG_askChoice4(void), *FIELDSTG_askChoice5(void), *FIELDSTG_askChoice6(void), *FIELDSTG_askChoice7(void);
void *FIELDSTG_askChoice8(void), *FIELDSTG_askChoice9(void), *FIELDSTG_askChoice10(void), *FIELDSTG_askChoice11(void);
void *FIELDSTG_askChoice12(void), *FIELDSTG_askChoice13(void), *FIELDSTG_askChoice14(void), *FIELDSTG_askChoice15(void);

/* story.c */
void FIELDSTG_runStoryEvents(StoryEvents *task, StoryEventsChildren *children);
StoryEvents *FIELDSTG_createStoryEvents(void *owner);

/* event.c */
Actor *FIELDSTG_findEventActor(EventTask *task, s32 id);
s32 FIELDSTG_setEventPose(EventTask *task, s16 *op, EventChildren *children);
void FIELDSTG_runEvent(EventTask *task, EventChildren *children);

/* cutscene.c */
void FIELDSTG_playCutsceneAnim(CutsceneAnim *task);
CutsceneAnim *FIELDSTG_createCutsceneAnim(s32 kind);

/* effect.c */
s32 FIELDSTG_stepEffectAnim(EffectAnim *anim, AnimFrame *frames, s32 depth);
void FIELDSTG_updateEffect(FieldEffect *task);
FieldEffect *FIELDSTG_createEffect(s32 x, s32 y, s32 set);

/* map_streamer.c */
StreamTask *FIELDSTG_findOldestStream(StreamPool *pool);
void FIELDSTG_requestTiles(MapStreamer *task, StreamPool *pool);
void FIELDSTG_drawCoverBlock(Layer *layer, s32 x, s32 y, s32 level);
StreamTask *FIELDSTG_findStream(StreamPool *pool, s32 frame);
void FIELDSTG_drawMapTiles(MapStreamer *task, StreamPool *pool);
void FIELDSTG_pickViewTiles(MapStreamer *task);
void FIELDSTG_runMapStreamer(MapStreamer *task, StreamPool *pool);
Vec2 *FIELDSTG_getMapSize(MapStreamer *task);
MapStreamer *FIELDSTG_createMapStreamer(s32 file);
void FIELDSTG_drawCover(s32 id, s32 level);

/* stream.c */
void FIELDSTG_touchStream(StreamTask *task);
void FIELDSTG_seekStream(StreamTask *task, s32 frame, s32 size);
s32 FIELDSTG_isStreamLoaded(StreamTask *task);
void FIELDSTG_drawStream(StreamTask *task, Layer *layer, s32 x, s32 y);
void FIELDSTG_setStreamSource(StreamTask *task, s32 slot, Decompressor *source);
void FIELDSTG_loadStreamSprites(StreamTask *task);
void FIELDSTG_clearStreamSlot(StreamTask *task);
s32 FIELDSTG_getStreamSlot(StreamTask *task);
s32 FIELDSTG_getStreamFrame(StreamTask *task);
void FIELDSTG_updateStream(StreamTask *task);
StreamTask *FIELDSTG_createStream(s32 size, s32 file);

/* start.c */
void FIELDSTG_updateRoot(Task *task, Task **children);

/* banner.c */
void FIELDSTG_showAreaName(AreaBanner *task, AreaNameWindows *windows);
void FIELDSTG_stretchBannerBox(AreaBanner *task, BannerBox *box);
void FIELDSTG_drawBannerBox(AreaBanner *task, u_long *ot, DVECTOR pos, DVECTOR size, s32 color);
void FIELDSTG_updateBanner(AreaBanner *task, AreaNameWindows *windows);
Task *FIELDSTG_createBanner(s32 show);

/* balloon.c */
s32 FIELDSTG_stepBalloonAnim(Balloon *task);
void FIELDSTG_drawBalloon(Balloon *task);
void FIELDSTG_updateBalloon(Balloon *task);
Balloon *FIELDSTG_createBalloon(s32 kind, s32 anim, s32 id);
void FIELDSTG_createPlayerBalloon(s32 id);
void FIELDSTG_balloonCommand(Balloon *task, s32 command, s32 id);

/* trigger.c */
s32 FIELDSTG_findTrigger(Triggers *task);
s32 FIELDSTG_offerTrigger(Triggers *task, TriggerChildren *children);
void FIELDSTG_setOffTrigger(Triggers *task);
void FIELDSTG_updateTriggers(Triggers *task, TriggerChildren *children);
Triggers *FIELDSTG_createTriggers(s32 sheetEntry, StageSlot *slots);

/* speech.c */
void FIELDSTG_getSpeechPos(Speech *task, Vec2 *out);
void FIELDSTG_updateSpeech(Speech *task, void **box);
Speech *FIELDSTG_createSpeech(Actor *actor, s32 entry, s32 type, s32 isMessage);
Speech *FIELDSTG_createTalk(Actor *actor, s32 entry);

/* map_objects.c */
void FIELDSTG_drawMapObject(MapObjects *task, Layer *layer, s32 index);
void FIELDSTG_updateMapObjects(MapObjects *task, HiddenSpots **children);
MapObjects *FIELDSTG_createMapObjects(s32 sprites, StageTile *objects);

/* file_loader.c */
void FIELDSTG_requestInnNames(Task *task);
void FIELDSTG_requestSlotFiles(Task *task);
s32 FIELDSTG_loadFieldFiles(Task *task);
void FIELDSTG_runFileLoader(Task *task);
Task *FIELDSTG_createFileLoader(s32 step);

/* actor_icon.c */
void FIELDSTG_updateActorIcon(ActorIcon *task);
ActorIcon *FIELDSTG_createActorIcon(Actor *actor);

/* field.c */
void FIELDSTG_playBattleTransition(FieldTask *task, FieldChildren *children);
void FIELDSTG_closeField(FieldTask *task, FieldChildren *children);
s32 FIELDSTG_keepsFileCache(void);
void FIELDSTG_updateField(FieldTask *task, FieldChildren *children);
Task *FIELDSTG_createField(void);
void FIELDSTG_leaveFieldAfter(s32 mode, s32 arg, s32 x, s32 y, s32 dir, s32 delay);
void FIELDSTG_startEncounter(s32 encounter);
void FIELDSTG_startWarp(s32 kind, Vec2 *pos, SlotDest *dest);

/* launch.c */
s32 FIELDSTG_scaleSin(s32 angle, s32 radius);
void FIELDSTG_runLaunch(Launch *task);
Launch *FIELDSTG_createLaunch(Actor *actor, SlotDest *dest);

/* hidden_spots.c */
void FIELDSTG_showSpotHint(SpotHint *task);
SpotHint *FIELDSTG_createSpotHint(Vec2 from, Vec2 to);
void FIELDSTG_hidePrize(HiddenSpots *task);
void FIELDSTG_updateHiddenSpots(HiddenSpots *task, HiddenSpotsChildren *children);
HiddenSpots *FIELDSTG_createHiddenSpots(s32 count);
HiddenSpots *FIELDSTG_findHiddenSpot(Vec2 *pos, s32 select);
void FIELDSTG_searchEventSpot(void);
void FIELDSTG_drawSpotEffect(SpotEffect *task, Layer *layer);
void FIELDSTG_updateSpotEffect(SpotEffect *task);
SpotEffect *FIELDSTG_createSpotEffect(s32 forEvent);

/* gauge.c */
void FIELDSTG_runGauge(GaugeGame *task);
GaugeGame *FIELDSTG_createGauge(Vec2 pos);

/* camera.c */
void FIELDSTG_scrollCamera(Camera *task);
void FIELDSTG_updateCamera(Camera *task);
Camera *FIELDSTG_createCamera(void);
void FIELDSTG_followWithCamera(s32 snap, s32 id);
void FIELDSTG_pointCamera(s32 snap, s32 x, s32 y);
void FIELDSTG_shakeCamera(s32 shaking);

/* actor_control.c */
s32 FIELDSTG_checkFlightProbe(Actor *actor, s32 x, s32 y, Vec2 offset);
s32 FIELDSTG_checkFlightProbes(Actor *actor);
void FIELDSTG_moveByPad(Actor *actor, s32 pad);
Actor *FIELDSTG_findActorAt(Vec2 *pos);
s32 FIELDSTG_talkToActorAt(Actor *actor, Vec2 *pos);
void FIELDSTG_controlFlight(Actor *actor);
void FIELDSTG_controlPlayer(Actor *actor);
void FIELDSTG_controlClimb(Actor *actor);
void FIELDSTG_followLeader(Actor *actor);
void FIELDSTG_drainTrail(Actor *actor);
void FIELDSTG_walkToGoal(Actor *actor);
void FIELDSTG_setActorGoal(Actor *actor, s32 x, s32 y, s32 dir);
s32 FIELDSTG_isActorWalking(Actor *actor);
void FIELDSTG_startActorWalk(Actor *actor);
void FIELDSTG_resetActorControl(Actor *actor);
void FIELDSTG_walkActorInDir(Actor *actor, s32 dir);
void FIELDSTG_startActorSlide(Actor *actor, s32 dir);
void FIELDSTG_stopActorSlide(Actor *actor);
void FIELDSTG_startClimbUp(Actor *actor, s32 dir, s32 x, s32 y, s32 height);
void FIELDSTG_startClimbDown(Actor *actor, s32 dir, s32 x, s32 y, s32 height);
void FIELDSTG_startDrop(Actor *actor, s32 dir, Vec2 pos, s32 height);
void FIELDSTG_startActorGauge(Actor *actor, s32 dir, Vec2 offset);
void FIELDSTG_warpActor(Actor *actor, SlotDest *dest, s32 kind);
void FIELDSTG_launchActor(Actor *actor, SlotDest *dest);
void FIELDSTG_setActorAnim(Actor *actor, s32 set);
void FIELDSTG_setActorPose(Actor *actor, s32 set, s32 dir);
s32 FIELDSTG_isActorAnimDone(Actor *actor);
void FIELDSTG_drawActor(void *data, void *drawLayer); /* (actor, layer) */
void FIELDSTG_setActorDir(Actor *actor, s32 dir);
void FIELDSTG_animateActor(Actor *actor);
void FIELDSTG_restorePlayerControl(Actor *actor);
void FIELDSTG_playStepSounds(Actor *actor, s32 moving, s32 canBattle);
void FIELDSTG_playClimbSounds(Actor *actor);
void FIELDSTG_runActorAction(Actor *actor, ActorChildren *children);

/* actor.c */
void FIELDSTG_haltPartners(void);
void FIELDSTG_resumePartners(void);
void FIELDSTG_getFacingTile(Actor *actor, Vec2 *out);
void FIELDSTG_updateActor(Actor *actor, ActorChildren *children);
Actor *FIELDSTG_createActor(s32 key1, s32 kind, s32 image, FieldActorEntry *entry);

/* field_stage.c */
void FIELDSTG_endChoice0Answer0(void), FIELDSTG_endChoice0Answer1(void), FIELDSTG_endChoice1Answer0(void), FIELDSTG_endChoice1Answer1(void);
void FIELDSTG_endChoice2Answer0(void), FIELDSTG_endChoice2Answer1(void), FIELDSTG_endChoice3Answer0(void), FIELDSTG_endChoice3Answer1(void);
void FIELDSTG_endChoice4Answer0(void), FIELDSTG_endChoice4Answer1(void), FIELDSTG_endChoice5Answer0(void), FIELDSTG_endChoice5Answer1(void);
void FIELDSTG_endChoice6Answer0(void), FIELDSTG_endChoice6Answer1(void), FIELDSTG_endChoice7Answer0(void), FIELDSTG_endChoice7Answer1(void);
void FIELDSTG_endChoice8Answer0(void), FIELDSTG_endChoice8Answer1(void), FIELDSTG_endChoice9Answer0(void), FIELDSTG_endChoice9Answer1(void);
void FIELDSTG_endChoice10Answer0(void), FIELDSTG_endChoice10Answer1(void), FIELDSTG_endChoice11Answer0(void), FIELDSTG_endChoice11Answer1(void);
void FIELDSTG_endChoice12Answer0(void), FIELDSTG_endChoice12Answer1(void), FIELDSTG_endChoice13Answer0(void), FIELDSTG_endChoice13Answer1(void);
void FIELDSTG_endChoice14Answer0(void), FIELDSTG_endChoice14Answer1(void), FIELDSTG_endChoice15Answer0(void), FIELDSTG_endChoice15Answer1(void);
void FIELDSTG_setupField(void);

/* tween.c */
void FIELDSTG_startTween(PanelAnim *tween, s32 in);
s32 FIELDSTG_updateTween(PanelAnim *tween);

/* state.c */
s32 FIELDSTG_getFileEntry(s32 index);
s32 FIELDSTG_getActorWidth(s32 index);
void FIELDSTG_pickStage(void);
FieldBattles *FIELDSTG_findBattles(FieldBattles *list, s32 id);

/* script.c */
void FIELDSTG_resetScriptTimer(void);
Actor *FIELDSTG_findActor(s32 character);
void FIELDSTG_waitScriptTime(s32 time, s32 *pc);
void FIELDSTG_waitAnimDone(s32 id, s32 *pc);
void FIELDSTG_waitWalkDone(s32 id, s32 *pc);
void FIELDSTG_toScreenPos(Vec2 *pos);
void FIELDSTG_clearScriptFlag(void);
ScriptCommand *FIELDSTG_findScriptCommand(s32 id);
s32 FIELDSTG_createScriptCommand(s32 id);
void FIELDSTG_handleScriptCommand(s32 task, s32 id, s32 command, s32 arg);

/* battle.c */
void FIELDSTG_rollBattleSteps(void);
void FIELDSTG_startAreaBattle(void);
void FIELDSTG_countBattleSteps(void);
void FIELDSTG_startEventBattle(s32 index);

/* map.c */
s32 FIELDSTG_selectMap(s32 index);
void FIELDSTG_setMapFile(s32 index, s32 value);
void FIELDSTG_setFirstMap(s32 index);
void FIELDSTG_setMap(s32 index);
s32 FIELDSTG_getMapCell(s32 index, Vec2 *pos);
s32 FIELDSTG_isTileFree(Vec2 *pos);
void FIELDSTG_getWalkStep(Vec2 *pos, s32 scale, s32 index, Vec2 *out);
void FIELDSTG_getFlyStep(Vec2 *pos, s32 scale, s32 index, Vec2 *out);

/* FIELDSTG's data (fieldstg.c), in its order */
extern Encounter FIELDSTG_encounters[];
extern s16 FIELDSTG_liftShake[]; /* FIELDSTG_updateLift's shakes, up to 1000 */
extern ChoiceText FIELDSTG_choices[16];
extern ProgressEvent FIELDSTG_progressEvents[];
extern AnimFrame FIELDSTG_cutsceneAnim[]; /* FIELDSTG_playCutsceneAnim's animations: kind 0's, */
extern AnimFrame FIELDSTG_cutsceneLoopAnim[]; /* and kind 1's two */
extern AnimFrame FIELDSTG_cutsceneLoopAnim2[];
extern AnimFrame *FIELDSTG_effectAnims[][4]; /* FIELDSTG_updateEffect's animations of each set */
extern u8 FIELDSTG_slotLayouts[][4][5][6]; /* FIELDSTG_pickViewTiles's slot layouts: [layout][quadrant][row][column] */
extern u8 FIELDSTG_dirLayouts[][2]; /* the layout and its flips for each direction */
extern u8 FIELDSTG_slotOffsets[][2][2]; /* the slots' offset in tiles: [quadrant][x, y][unflipped, flipped] */
extern s32 FIELDSTG_spriteDepths[]; /* depth of each layer of a StreamTask's sprites */
extern Vec2 FIELDSTG_slotImages[]; /* VRAM position of each StreamTask slot's image */
extern AreaName FIELDSTG_areaNames[];
extern BannerBox FIELDSTG_bannerBoxes[10]; /* FIELDSTG_updateBanner's boxes */
extern u8 FIELDSTG_triggerAnims[][9]; /* FIELDSTG_stepBalloonAnim's animations: (frame, time) pairs up to 0xFF */
extern u8 FIELDSTG_nearDirs[][8]; /* whether two directions are at most 45 degrees apart */
extern s16 FIELDSTG_file5DModes[]; /* the modes that load the field file 0x5D (FIELDSTG_requestInnNames) */
extern u8 (*FIELDSTG_iconAnims[])[2]; /* FIELDSTG_updateActorIcon's animation of each substate */
extern IconOffset FIELDSTG_iconPath[]; /* up to (0, 0) */
extern s32 FIELDSTG_iconPathStep; /* the step in FIELDSTG_iconPath */
extern TileMove FIELDSTG_tileMoves[];
extern s16 FIELDSTG_eventIds[]; /* the events FIELDSTG_startListedEvent starts */
extern u8 FIELDSTG_spotAnim[][2]; /* animation of FIELDSTG_updateHiddenSpots: (frame, time) pairs up to 0xFF */
extern u8 *FIELDSTG_dirAnims[]; /* FIELDSTG_updateSpotEffect's animation for each direction */
extern s32 FIELDSTG_dirDepths[]; /* and its depth offset */
extern u8 *FIELDSTG_gaugeRows[];
extern Vec2 FIELDSTG_shakeOffsets[]; /* the camera's shake offsets */
extern u8 FIELDSTG_probes[][5]; /* the probes of each direction (FIELDSTG_checkFlightProbes) */
extern Vec2 FIELDSTG_probePos[]; /* a probe's position */
extern u8 FIELDSTG_probeSteps[][2]; /* a probe's offset: bit 0 set, bit 7 negative */
extern s32 FIELDSTG_padDirs[]; /* the direction of each combination of the pad directions */
extern StandIn FIELDSTG_standIns[]; /* up to character 0 */
/* the partners' kinds (Actor.key2): the ones FIELDSTG_startActorGauge turns, FIELDSTG_haltPartners
   stops and FIELDSTG_resumePartners makes follow again */
extern s32 FIELDSTG_turnedPartners[];
extern s32 FIELDSTG_haltedPartners[];
extern s32 FIELDSTG_followingPartners[];
extern Vec2 FIELDSTG_dirSteps[]; /* tile offset of each direction */
/* the field's own stage (FIELDSTG_setupField) */
extern FieldImages FIELDSTG_images;
extern FieldActorEntry *FIELDSTG_actorList[];
extern StageTile FIELDSTG_mapObjects[];
extern StageSlot FIELDSTG_slots[];
extern void (*FIELDSTG_initFuncs[])(void);
extern void (*FIELDSTG_tweenStart)(PanelAnim *tween, s32 in); /* FIELDSTG_startTween */
extern s32 (*FIELDSTG_tweenUpdate)(PanelAnim *tween); /* FIELDSTG_updateTween */
extern FieldEvent FIELDSTG_events[];
extern s32 FIELDSTG_fileEntries[]; /* by character (Actor.key1) */
extern u8 FIELDSTG_actorWidths[]; /* by character, in pixels (FieldState.getActorWidth) */
#if VERSION_EU || VERSION_JP
extern StageEntry FIELDSTG_extraStages[]; /* the extra chapter's, which run on into FIELDSTG_stages */
#endif
extern StageEntry FIELDSTG_stages[];
extern ScriptTimer FIELDSTG_scriptTimer;
extern void (*FIELDSTG_scriptHelpers[])(); /* the script helpers (FIELDSTG_toScreenPos...) */
extern ScriptCommand FIELDSTG_scriptCommands[];
extern void (*FIELDSTG_checkBattle)(); /* FIELDSTG_countBattleSteps */
extern s32 FIELDSTG_battleRates[]; /* how much each area lowers GAME.battleSteps, the steps to the next battle */
extern s32 FIELDSTG_boxFrame; /* the frame FIELDSTG_boxes was filled in */
extern Vec2 FIELDSTG_dirVectors[][8]; /* a direction's vector, scaled by 4096 */
extern u8 FIELDSTG_mirrorDirs[];
extern s16 FIELDSTG_slideVoice; /* the voice of the slide's sound (ACTOR_SLIDE, SOUND_TRAP_ICE) */
extern s16 FIELDSTG_heldVoice; /* the voice of FIELDSTG_handleFieldCommand's held sound */
extern Vec2 FIELDSTG_mapSize; /* FIELDSTG_getMapSize's */
extern StageTile *FIELDSTG_objectCursor; /* FIELDSTG_findNextObject's search of the map objects */
extern s32 FIELDSTG_objectId; /* and the id it looks for */
extern Vec2 FIELDSTG_tiles[TRANSITION_COLUMNS][TRANSITION_ROWS]; /* FIELDSTG_playBattleTransition's */
extern s32 FIELDSTG_battleFileStep; /* FIELDSTG_playBattleTransition's: 0 and 1 wait for the CD, 2 the battle's files are asked for */
extern RECT FIELDSTG_screenRect;
extern Box FIELDSTG_boxes[20]; /* the characters' boxes (FIELDSTG_isTileFree) */
extern s32 FIELDSTG_boxCount; /* and their number */

/* Starts a battle: its fight stage, its music and its encounter */
static inline void FIELDSTG_startBattle(AreaBattle *battle) {
    BATTLE_SETUP.stage = battle->stage;
    BATTLE_SETUP.music = battle->music;
    FIELDSTG_startEncounter(battle->encounter);
}

#endif /* FIELDSTG_H */
