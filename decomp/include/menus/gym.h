#ifndef STGTRAIN_H
#define STGTRAIN_H

/* STGTRAIN.PRO: the gyms, where the partners train their stats. The mode
   it was entered from (the gym's stage) picks the gym's sign; the screen
   shows the party, the selected partner's stats and the training menu. */

#include "engine/game.h"

/* The name of this overlay's copy of a function of src/shared/ */
#define OVL_NAME(name) STGTRAIN_##name

/* The overlay's files: the discs number them differently */
#if VERSION_US || VERSION_JP
#define STGTRAIN_TEXT TEXT_FILE(0x105) /* its strings */
#define STGTRAIN_FILE_SPRITES 0x27D
#define STGTRAIN_FILE_IMAGES 0x27E /* a TIM archive */
#elif VERSION_EU
#define STGTRAIN_TEXT (LANGUAGE + 0x10B)
#define STGTRAIN_FILE_SPRITES 0x28C
#define STGTRAIN_FILE_IMAGES 0x28D
#endif
#define STGTRAIN_SPRITES (STGTRAIN_FILE_SPRITES << 16 | 3) /* its sprite bank */

/* A frame of a sprite animation */
typedef struct TrainAnimFrame {
    /* 0x0 */ s16 sprite; /* into the sprite bank */
    /* 0x2 */ u8 duration; /* in ticks of GFX.funcs.getTime */
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 y;
} TrainAnimFrame;

/* A sprite animation: its frames follow the header */
typedef struct TrainAnim {
    /* 0x0 */ s16 unk0; /* 0x124 in every animation; nothing reads it */
    /* 0x2 */ s16 frameCount;
    /* 0x4 */ TrainAnimFrame frames[1];
} TrainAnim;

/* A part of a sprite: a rectangle of a texture page. A sprite is a count
   and its parts, and the bank's sprites follow each other. */
typedef struct TrainSpritePart {
    /* 0x00 */ u8 u;
    /* 0x01 */ u8 v;
    /* 0x02 */ u8 x;
    /* 0x03 */ u8 y;
    /* 0x04 */ u16 clut; /* bits 6-14: the CLUT's row; bit 15: semi-transparent */
    /* 0x06 */ u16 tpage; /* bits 0-4: the page's column, 64 pixels each; 5-8: getTPage's abr and tp */
    /* 0x08 */ u16 w;
    /* 0x0A */ u16 h;
    /* 0x0C */ u8 unkC[8]; /* nothing reads it: in the files, four s16: 0, flags
                             (bits 0, 14 and 15), 0x1000 and 0x1000 */
} TrainSpritePart;

/* A sprite bank's header, as the animated sprite reads it: its sprites
   start bankOffset bytes from it (TrainImageSet) */
typedef struct TrainSpriteBank {
    /* 0x0 */ s16 unk0; /* 801 in every bank; nothing reads it */
    /* 0x2 */ u16 unk2; /* 0 in every bank */
    /* 0x4 */ u16 spriteCount;
} TrainSpriteBank;

/* Draws an animated sprite, maybe rotated and scaled (STGTRAIN_createSprite) */
typedef struct TrainSprite {
    TASK_HEADER(TrainSprite);
    /* 0x050 */ Layer *layer;
    /* 0x054 */ u_long *ot;
    /* 0x058 */ s32 layerId;
    /* 0x05C */ s32 depth;
    /* 0x060 */ s32 x;
    /* 0x064 */ s32 y;
    /* 0x068 */ s32 imageX;
    /* 0x06C */ s32 imageY;
    /* 0x070 */ s32 clutX;
    /* 0x074 */ s32 clutY;
    /* 0x078 */ s32 flags; /* bit 31: paused; the rest, ended */
    /* 0x07C */ s32 pad; /* never read or written */
    /* 0x080 */ TrainSpriteBank *bank;
    /* 0x084 */ u16 unk84; /* the bank's unk2 and spriteCount: nothing reads them */
    /* 0x086 */ u16 spriteCount;
    /* 0x088 */ s32 bankOffset;
    /* 0x08C */ TrainAnim *anim;
    /* 0x090 */ s32 frame;
    /* 0x094 */ s32 frameCount;
    /* 0x098 */ s32 frameTime;
    /* 0x09C */ s32 transformed;
    /* 0x0A0 */ s32 pivotX;
    /* 0x0A4 */ s32 pivotY;
    /* 0x0A8 */ VECTOR scale;
    /* 0x0B8 */ SVECTOR rotation;
    /* 0x0C0 */ MATRIX matrix;
    /* 0x0E0 */ void (*setBank)(struct TrainSprite *sprite, TrainSpriteBank *bank, s32 offset);
    /* 0x0E4 */ void (*setAnim)(struct TrainSprite *sprite, TrainAnim *anim);
    /* 0x0E8 */ void (*setPos)(struct TrainSprite *sprite, s32 x, s32 y);
    /* 0x0EC */ void (*setImagePos)(struct TrainSprite *sprite, s32 x, s32 y);
    /* 0x0F0 */ void (*setLayer)(struct TrainSprite *sprite, s32 layerId, s32 depth);
    /* 0x0F4 */ void (*setClutPos)(struct TrainSprite *sprite, s32 x, s32 y);
    /* 0x0F8 */ void (*setScale)(struct TrainSprite *sprite, s32 x, s32 y, s32 z);
    /* 0x0FC */ void (*setPivot)(struct TrainSprite *sprite, s32 x, s32 y);
    /* 0x100 */ void (*setRotation)(struct TrainSprite *sprite, s16 x, s16 y, s16 z);
    /* 0x104 */ s32 (*getFlags)(struct TrainSprite *sprite);
    /* 0x108 */ void (*setPaused)(struct TrainSprite *sprite, s32 paused);
} TrainSprite;

/* The training screen's main task (STGTRAIN_createScreen) */
typedef struct TrainScreen {
    TASK_HEADER(TrainScreen);
    /* 0x050 */ s32 layerId;
    /* 0x054 */ s32 depth;
    /* 0x058 */ s32 bgSprite; /* the gym's sign, by the mode it came from */
    /* 0x05C */ s32 bgScroll;
    /* 0x060 */ s32 bgTick;
    /* 0x064 */ s32 partyCount;
    /* 0x068 */ s32 partner; /* the one selected */
    /* 0x06C */ s32 cursorShown;
    /* 0x070 */ s32 cursorClut;
    /* 0x074 */ s32 cursorTime;
    /* 0x078 */ s32 training; /* the one picked in the menu, 0 for none */
    /* 0x07C */ s32 intensity; /* the last one picked */
    /* 0x080 */ struct {
        s32 frame;
        s32 time;
    } anims[3]; /* the party's sprites */
    /* 0x098 */ s32 frame; /* the selected partner's sprite */
    /* 0x09C */ s32 time;
    /* 0x0A0 */ PanelAnim panels[7];
    /* 0x110 */ void (*showStats)(struct TrainScreen *screen, PartnerTotals *before);
} TrainScreen;

/* The children of the training screen */
typedef struct TrainScreenWindows {
    /* 0x00 */ TextWindow *name;
    /* 0x04 */ TextWindow *level[2];
    /* 0x0C */ TextWindow *hp[3];
    /* 0x18 */ TextWindow *mp[3];
    /* 0x24 */ TextWindow *slashes[2];
    /* 0x2C */ TextWindow *stats[6];
    /* 0x44 */ TextWindow *resistances[7];
    /* 0x60 */ TextWindow *tp[2];
    /* 0x68 */ TextWindow *help; /* triangle stops training (string 5) */
    /* 0x6C */ TextWindow *prompt; /* "Train which Digimon?" (string 6) */
    /* 0x70 */ struct TrainMenu *menu;
    /* 0x74 */ struct TrainSession *session;
    /* 0x78 */ struct TrainResult *result;
    /* 0x7C */ ScreenFade *fade;
} TrainScreenWindows;

/* The training menu (STGTRAIN_createMenu) */
typedef struct TrainMenu {
    TASK_HEADER(TrainMenu);
    /* 0x050 */ TrainScreen *screen;
    /* 0x054 */ s32 layerId;
    /* 0x058 */ s32 depth;
    /* 0x05C */ s32 cursorShown;
    /* 0x060 */ s32 cursorClut;
    /* 0x064 */ s32 cursorTime;
    /* 0x068 */ s32 col; /* the cursor's */
    /* 0x06C */ s32 row;
    /* 0x070 */ s32 page;
    /* 0x074 */ s32 arrowShown; /* to the other page */
    /* 0x078 */ s32 arrowClut;
    /* 0x07C */ s32 arrowTime;
    /* 0x080 */ s32 iconFrame; /* the selected training's icon */
    /* 0x084 */ s32 iconTime;
    /* 0x088 */ s32 trainings[2][8]; /* [page][row * 4 + col], -1: none */
    /* 0x0C8 */ PanelAnim panels[4];
    /* 0x108 */ void (*open)(struct TrainMenu *menu);
    /* 0x10C */ void (*close)(struct TrainMenu *menu);
    /* 0x110 */ void (*showInfo)(struct TrainMenu *menu);
} TrainMenu;

/* A training session (STGTRAIN_createSession) */
typedef struct TrainSession {
    TASK_HEADER(TrainSession);
    /* 0x050 */ TrainScreen *screen;
    /* 0x054 */ s32 layerId;
    /* 0x058 */ s32 depth;
    /* 0x05C */ s32 intensity; /* the cursor's column, the training's intensity; from the screen's */
    /* 0x060 */ s32 cursorClut;
    /* 0x064 */ s32 cursorTime;
    /* 0x068 */ s32 cursorShown;
    /* 0x06C */ s32 choice; /* yes (0) or no */
    /* 0x070 */ s32 iconFrame;
    /* 0x074 */ s32 iconTime;
    /* 0x078 */ PanelAnim panels[8];
    /* 0x0F8 */ void (*finish)(struct TrainSession *session);
} TrainSession;

/* The children of a training session */
typedef struct TrainSessionWindows {
    /* 0x00 */ TextWindow *text[3]; /* the message, the training and the intensity */
    /* 0x0C */ TextWindow *intensities[3];
    /* 0x18 */ TextWindow *notice; /* not enough points */
    /* 0x1C */ TextWindow *answers[2]; /* yes, no */
    /* 0x24 */ Cursor *cursor;
} TrainSessionWindows;

/* A task the overlay creates (STGTRAIN_createIdle) that does nothing */
typedef struct TrainIdle {
    TASK_HEADER(TrainIdle);
    /* 0x50 */ TrainScreen *screen;
    /* 0x54 */ s32 layerId;
    /* 0x58 */ s32 depth;
    /* 0x5C */ u8 pad[0x10]; /* never read or written */
} TrainIdle;

/* An animated sprite of an image set, as a training shows it
   (STGTRAIN_createActor): mode 1 idle, 2 scaling, 4 animating, 8 ending */
typedef struct TrainActor {
    TASK_HEADER(TrainActor);
    /* 0x50 */ u32 mode;
    /* 0x54 */ s32 set;
    /* 0x58 */ s32 file; /* of STGTRAIN_files */
    /* 0x5C */ s32 anim;
    /* 0x60 */ s32 result; /* the training worked */
    /* 0x64 */ s32 scale;
    /* 0x68 */ s32 scaleStep;
    /* 0x6C */ s32 pad6C; /* never read or written */
    /* 0x70 */ s32 scaleChanged;
    /* 0x74 */ s32 layerId;
    /* 0x78 */ s32 depth;
    /* 0x7C */ s32 pad7C; /* never read or written */
    /* 0x80 */ s32 pos[4]; /* x, y, clutX, clutY: for loadSet */
    /* 0x90 */ s32 savedX;
    /* 0x94 */ s32 posSet;
    /* 0x98 */ s32 clutSet;
    /* 0x9C */ s32 chance; /* of the training working, in % */
    /* 0xA0 */ s32 padA0; /* never read or written */
    /* 0xA4 */ void (*setPos)(struct TrainActor *actor, s32 x, s32 y);
    /* 0xA8 */ void (*setClutPos)(struct TrainActor *actor, s32 x, s32 y);
    /* 0xAC */ void (*setChance)(struct TrainActor *actor, s32 chance);
    /* 0xB0 */ void (*grow)(struct TrainActor *actor);
    /* 0xB4 */ void (*shrink)(struct TrainActor *actor);
    /* 0xB8 */ void (*play)(struct TrainActor *actor);
    /* 0xBC */ void (*pause)(struct TrainActor *actor);
    /* 0xC0 */ void (*setScale)(struct TrainActor *actor, s32 scale);
    /* 0xC4 */ s32 (*getResult)(struct TrainActor *actor);
    /* 0xC8 */ void (*end)(struct TrainActor *actor);
    /* 0xCC */ u8 padCC[8]; /* never read or written */
} TrainActor;

/* The children of a TrainActor */
typedef struct TrainActorSprites {
    /* 0x0 */ TrainSprite *sprite;
    /* 0x4 */ TrainSprite *effect; /* of set 8 */
    /* 0x8 */ void *pad; /* never read or written */
} TrainActorSprites;

/* The results of a training (STGTRAIN_createResult) */
typedef struct TrainResult {
    TASK_HEADER(TrainResult);
    /* 0x050 */ TrainScreen *screen;
    /* 0x054 */ s32 layerId;
    /* 0x058 */ s32 depth;
    /* 0x05C */ s32 partner;
    /* 0x060 */ s32 training;
    /* 0x064 */ s32 trained[5];
    /* 0x078 */ s32 gains[5];
    /* 0x08C */ s32 losses[5];
    /* 0x0A0 */ s32 modeArg;
    /* 0x0A4 */ PartnerTotals before;
    /* 0x0D4 */ s32 bonusTrying; /* the bonus try (the fourth) is on: its mark blinks */
    /* 0x0D8 */ s32 bonusWorked; /* the gains take their better columns */
    /* 0x0DC */ s32 bonusBlink;
    /* 0x0E0 */ s32 bonusBlinkTime;
    /* 0x0E4 */ s32 prompting; /* the blinking arrow: cross goes on */
    /* 0x0E8 */ s32 arrowClut;
    /* 0x0EC */ s32 arrowTime;
    /* 0x0F0 */ s16 bonusSound; /* the slot of the bonus try's sound */
    /* 0x0F4 */ PanelAnim panels[4];
} TrainResult;

/* The children of TrainResult */
typedef struct TrainResultWindows {
    /* 0x00 */ TextWindow *message[2];
    /* 0x08 */ TextWindow *question[3]; /* whether to make the bonus try, yes, no */
    /* 0x14 */ Cursor *cursor;
    /* 0x18 */ TrainActor *actor;
} TrainResultWindows;

/* A stat change of a training: base plus a random 0..range-1 */
typedef struct TrainGain {
    /* 0x0 */ s32 base;
    /* 0x4 */ s32 range;
} TrainGain;

/* An entry of STGTRAIN_files: a file and where its image goes */
typedef struct TrainFile {
    /* 0x0 */ s32 file;
    /* 0x4 */ s32 x;
    /* 0x8 */ s32 y;
    /* 0xC */ s32 effectSameDepth; /* 0 draws the effect one depth in front of the
                                      Digimon (STGTRAIN_updateActor) */
} TrainFile;

/* An image set of the loaded file (STGTRAIN_readSet) */
typedef struct TrainImageSet {
    /* 0x00 */ s32 id;
    /* 0x04 */ TrainSpriteBank *bank;
    /* 0x08 */ s32 bankOffset;
    /* 0x0C */ s32 bankSize; /* in bytes */
    /* 0x10 */ TrainAnim *anims[6];
    /* 0x28 */ u8 *images[4];
    /* 0x38 */ s32 imageCount;
    /* 0x3C */ s32 imageX[4];
    /* 0x4C */ s32 imageY;
} TrainImageSet;

/* The header of an image set in the loaded file: offsets into it, then
   the bank's offset of its first sprite and the bank's size */
typedef struct TrainSetHeader {
    /* 0x00 */ s32 count; /* of the offsets */
    /* 0x04 */ s32 bank;
    /* 0x08 */ s32 anims[6];
    /* 0x20 */ s32 images[1];
} TrainSetHeader;

/* Where readSet and its helpers read the loaded file */
typedef union TrainCursor {
    u8 *bytes; /* the file's offsets count bytes from its start */
    s32 *w;
    TrainSetHeader *set;
} TrainCursor;

/* An entry of a gym level's trainings (STGTRAIN_gymTrainings) */
typedef struct TrainEntry {
    /* 0x0 */ s32 id;
    /* 0x4 */ s16 stat; /* 1-5 battle stats, 8-14 resistances */
    /* 0x6 */ s16 other; /* 1-5: lowered, 15 or 16: max HP or MP raised */
} TrainEntry;

/* A training (STGTRAIN_trainings) */
typedef struct TrainInfo {
    /* 0x00 */ s32 name; /* strings of the text file */
    /* 0x04 */ s32 desc;
    /* 0x08 */ s32 icons[4]; /* frames of STGTRAIN_FILE_SPRITES */
} TrainInfo;

/* The overlay's helpers and the file they load (STGTRAIN_state) */
typedef struct TrainState {
    /* 0x000 */ s32 tableCount; /* the entries of the last getTable */
    /* 0x004 */ u8 *data; /* the loaded file of STGTRAIN_files */
    /* 0x008 */ s32 fileIndex;
    /* 0x00C */ TrainImageSet sets[9];
    /* 0x2DC */ TrainInfo *trainings; /* STGTRAIN_trainings */
    /* 0x2E0 */ void (*loadImages)(void);
    /* 0x2E4 */ void (*startFade)(PanelAnim *fade, s32 fadeIn);
    /* 0x2E8 */ s32 (*updateFade)(PanelAnim *fade);
    /* 0x2EC */ void (*startLerp)(MenuLerp *lerp, s32 from, s32 to, s32 frames);
    /* 0x2F0 */ s32 (*updateLerp)(MenuLerp *lerp);
    /* 0x2F4 */ s32 (*requestFile)(s32 index);
    /* 0x2F8 */ u8 *(*getFile)(void);
    /* 0x2FC */ void (*freeFile)(void);
    /* 0x300 */ s32 (*readSet)(s32 set);
    /* 0x304 */ s32 (*loadSet)(s32 set, s32 *pos);
    /* 0x308 */ s32 (*getFileId)(s32 index);
    /* 0x30C */ s32 (*getFilePos)(s32 index); /* y << 16 | x */
    /* 0x310 */ s32 (*getFileEffectSameDepth)(s32 index);
    /* 0x314 */ TrainSpriteBank *(*getBank)(s32 set);
    /* 0x318 */ s32 (*getBankOffset)(s32 set);
    /* 0x31C */ s32 (*getBankSize)(s32 set);
    /* 0x320 */ TrainAnim *(*getAnim)(s32 set, s32 i);
    /* 0x324 */ TrainEntry *(*getTable)(s32 index);
    /* 0x328 */ TrainEntry *(*findTableEntry)(s32 index, s32 id);
} TrainState;

extern TrainState STGTRAIN_state;

/* The overlay's data, in its order: the first object's, the second's and the
   third's */
extern s32 STGTRAIN_waitAnims[8][7];
extern TrainGain STGTRAIN_statGains[];
extern TrainGain STGTRAIN_statLosses[];
extern TrainGain *STGTRAIN_resistGainTables[];
extern TrainGain STGTRAIN_maxHpMpGains[];
extern s32 STGTRAIN_intensityCosts[]; /* the points each intensity of a training costs */
extern TrainEntry STGTRAIN_gymTrainings[14][16];
extern TrainFile STGTRAIN_files[];
extern TrainCursor STGTRAIN_bankCursor;
extern TrainCursor STGTRAIN_animCursor;
extern TrainCursor STGTRAIN_imageCursor;
extern TrainCursor STGTRAIN_setCursor;

/* sprite.c */
void STGTRAIN_setSpriteBank(TrainSprite *sprite, TrainSpriteBank *bank, s32 offset);
void STGTRAIN_setSpriteAnim(TrainSprite *sprite, TrainAnim *anim);
void STGTRAIN_setSpriteLayer(TrainSprite *sprite, s32 layerId, s32 depth);
void STGTRAIN_setSpritePos(TrainSprite *sprite, s32 x, s32 y);
void STGTRAIN_setSpriteImagePos(TrainSprite *sprite, s32 x, s32 y);
void STGTRAIN_setSpriteClutPos(TrainSprite *sprite, s32 x, s32 y);
void STGTRAIN_setSpriteScale(TrainSprite *sprite, s32 x, s32 y, s32 z);
void STGTRAIN_setSpritePivot(TrainSprite *sprite, s32 x, s32 y);
void STGTRAIN_setSpriteRotation(TrainSprite *sprite, s16 x, s16 y, s16 z);
s32 STGTRAIN_getSpriteFlags(TrainSprite *sprite);
void STGTRAIN_setSpritePaused(TrainSprite *sprite, s32 paused);
void STGTRAIN_updateSprite(TrainSprite *sprite);
TrainSprite *STGTRAIN_createSprite(void);

/* scene.c */
void STGTRAIN_updateRoot(Task *task, Task **children);

/* screen.c */
void STGTRAIN_createScreenWindows(TrainScreen *screen, TrainScreenWindows *win);
void STGTRAIN_showVitals(TrainScreen *screen, TrainScreenWindows *win, s32 show);
void STGTRAIN_showBattleStats(TrainScreen *screen, TrainScreenWindows *win, s32 show);
void STGTRAIN_showTp(TrainScreen *screen, TrainScreenWindows *win, s32 show);
void STGTRAIN_showStatChanges(TrainScreen *screen, PartnerTotals *before);
void STGTRAIN_drawScreen(TrainScreen *screen);
void STGTRAIN_runScreen(TrainScreen *screen, TrainScreenWindows *win);
void STGTRAIN_updateScreen(TrainScreen *screen, TrainScreenWindows *win);
TrainScreen *STGTRAIN_createScreen(void);
void STGTRAIN_startFader(ScreenFade *task, s32 fadeIn, s32 duration);
void STGTRAIN_drawFader(ScreenFade *task);
void STGTRAIN_updateFader(ScreenFade *task);
ScreenFade *STGTRAIN_createFader(void);

/* result.c */
s32 STGTRAIN_raiseStat(TrainResult *result, s32 stat);
s32 STGTRAIN_lowerStat(TrainResult *result, s32 stat);
s32 STGTRAIN_raiseResistance(TrainResult *result, s32 stat);
s32 STGTRAIN_raiseMaxHpMp(TrainResult *result, s32 stat);
void STGTRAIN_applyTry(TrainResult *result, s32 i);
s32 STGTRAIN_getAccessoryBonus(TrainResult *result);
void STGTRAIN_createResultWindows(TrainResult *result, TrainResultWindows *win);
void STGTRAIN_drawResult(TrainResult *result);
void STGTRAIN_runTraining(TrainResult *result, TrainResultWindows *win);
void STGTRAIN_updateResult(TrainResult *result, TrainResultWindows *win);
TrainResult *STGTRAIN_createResult(TrainScreen *screen, s32 partner, s32 training);
void STGTRAIN_initIdle(TrainIdle *task, void *children);
void STGTRAIN_showIdle(TrainIdle *task, void *children, s32 show);
void STGTRAIN_drawIdle(TrainIdle *task);
void STGTRAIN_runIdle(TrainIdle *task, void *children);
void STGTRAIN_updateIdle(TrainIdle *task, void *children);
TrainIdle *STGTRAIN_createIdle(TrainScreen *screen);

/* session.c */
void STGTRAIN_createSessionWindows(TrainSession *session, TrainSessionWindows *win);
void STGTRAIN_drawSession(TrainSession *session);
void STGTRAIN_runSession(TrainSession *session, TrainSessionWindows *win);
void STGTRAIN_updateSession(TrainSession *session, TrainSessionWindows *win);
void STGTRAIN_finishSession(TrainSession *session);
TrainSession *STGTRAIN_createSession(TrainScreen *screen);

/* actor.c */
void STGTRAIN_updateActor(TrainActor *actor, TrainActorSprites *sprites);
void STGTRAIN_pauseActor(TrainActor *actor);
void STGTRAIN_playActor(TrainActor *actor);
void STGTRAIN_setActorPos(TrainActor *actor, s32 x, s32 y);
void STGTRAIN_setActorClutPos(TrainActor *actor, s32 x, s32 y);
void STGTRAIN_setActorChance(TrainActor *actor, s32 chance);
void STGTRAIN_growActor(TrainActor *actor);
void STGTRAIN_shrinkActor(TrainActor *actor);
void STGTRAIN_setActorScale(TrainActor *actor, s32 scale);
s32 STGTRAIN_getActorResult(TrainActor *actor);
void STGTRAIN_endActor(TrainActor *actor);
TrainActor *STGTRAIN_createActor(s32 set, s32 file, s32 layerId, s32 depth);

/* menu.c */
void STGTRAIN_createMenuWindows(TrainMenu *menu, TextWindow **win);
void STGTRAIN_showTrainingInfo(TrainMenu *menu, TextWindow **win, s32 show);
void STGTRAIN_drawMenu(TrainMenu *menu);
void STGTRAIN_runMenu(TrainMenu *menu, TextWindow **win);
void STGTRAIN_updateMenu(TrainMenu *menu, TextWindow **win);
void STGTRAIN_openMenu(TrainMenu *menu);
void STGTRAIN_closeMenu(TrainMenu *menu);
void STGTRAIN_showMenuInfo(TrainMenu *menu);
TrainMenu *STGTRAIN_createMenu(TrainScreen *screen);

/* files.c */
void STGTRAIN_loadImages(void);
void STGTRAIN_startFade(PanelAnim *fade, s32 fadeIn);
s32 STGTRAIN_updateFade(PanelAnim *fade);
void STGTRAIN_startLerp(MenuLerp *lerp, s32 from, s32 to, s32 frames);
s32 STGTRAIN_updateLerp(MenuLerp *lerp);
s32 STGTRAIN_requestFile(s32 index);
u8 *STGTRAIN_getFile(void);
void STGTRAIN_freeFile(void);
s32 STGTRAIN_readSetBank(TrainSetHeader *header, s32 set);
s32 STGTRAIN_readSetAnims(TrainSetHeader *header, s32 set);
s32 STGTRAIN_readSetImages(TrainSetHeader *header, s32 set);
s32 STGTRAIN_readSet(s32 set);
s32 STGTRAIN_loadSet(s32 set, s32 *pos);
s32 STGTRAIN_getFileId(s32 index);
s32 STGTRAIN_getFilePos(s32 index);
s32 STGTRAIN_getFileEffectSameDepth(s32 index);
TrainSpriteBank *STGTRAIN_getBank(s32 set);
s32 STGTRAIN_getBankOffset(s32 set);
s32 STGTRAIN_getBankSize(s32 set);
TrainAnim *STGTRAIN_getAnim(s32 set, s32 i);
TrainEntry *STGTRAIN_getGymTrainings(s32 index);
TrainEntry *STGTRAIN_findGymTraining(s32 index, s32 id);

#endif /* STGTRAIN_H */
