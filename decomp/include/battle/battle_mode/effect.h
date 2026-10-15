#ifndef FIGHTSTG_EFFECT_H
#define FIGHTSTG_EFFECT_H

/* FIGHTSTG's effects and scripts: sprite effects and their animations, hit
   effects, battle scripts and sounds, the white flash, Digimon changes,
   entrances and the screen fader (sprite_anim.c to fader.c). */

#include "battle/battle_mode/types.h"
#include "battle/battle_mode/model.h"

/* The images of a Digimon's change (FIGHTSTG_updateDigimonChange): entry 5 */
#if VERSION_EU
#define FILE_CHANGE 0x8A2
#elif VERSION_US || VERSION_JP
#define FILE_CHANGE 0x891
#endif

/* What fighter 0x1D2's entrance (FIGHTSTG_updateEntrance) loads */
#if VERSION_EU
#define FILE_ENTRANCE_1D2 0x6E2
#elif VERSION_US || VERSION_JP
#define FILE_ENTRANCE_1D2 0x6D3
#endif

/* A battle sound (FIGHTSTG_playBattleSound) that is keyed off after time */
typedef struct BattleSound {
    TASK_HEADER(BattleSound);
    /* 0x50 */ s32 sound;
    /* 0x54 */ s32 voice;
    /* 0x58 */ s32 time;
} BattleSound;

/* A sprite sheet for the battle's 2D effects: an archive of animations,
   the sheet and where its texture goes in VRAM */
typedef struct EffectSheet {
    /* 0x0 */ s32 images; /* the archive of TIMs a script loads for it, 0 for none */
    /* 0x4 */ s32 sheet;
    /* 0x8 */ Vec2 texPos;
} EffectSheet;

/* A 2D effect: its sheet in FIGHTSTG_effectSheets and its animations, an
   archive of SpriteAnim data; the list (FIGHTSTG_spriteEffects) ends with an
   id of -1 */
typedef struct SpriteEffectEntry {
    /* 0x0 */ s16 id;
    /* 0x2 */ s16 sheet;
    /* 0x4 */ s32 file;
} SpriteEffectEntry;

/* A 2D effect (FIGHTSTG_startSpriteEffect): a SpriteAnim per animation, killed
   when they all are */
typedef struct SpriteEffect {
    TASK_HEADER(SpriteEffect);
    /* 0x50 */ s32 effect; /* -1: not found */
    /* 0x54 */ s32 sheet;
    /* 0x58 */ s32 file;
    /* 0x5C */ SVECTOR pos;
    /* 0x64 */ s32 count;
} SpriteEffect;

/* One animation of a 2D effect (FIGHTSTG_createSpriteAnim), drawn by a layer
   callback. Its data: flags, stride and duration, the 9 values of the first
   frame, then per frame the values whose bit is set in flags. */
typedef struct SpriteAnim {
    TASK_HEADER(SpriteAnim);
    /* 0x50 */ s16 *data;
    /* 0x54 */ SVECTOR pos; /* vz 0x7FFF: on screen in front, -1: on screen */
    /* 0x5C */ s32 sheet;
    /* 0x60 */ Vec2 texPos;
    /* 0x68 */ s32 layerId;
    /* 0x6C */ s32 time;
    /* 0x70 */ s32 flags;
    /* 0x74 */ s32 stride; /* of a frame's values */
    /* 0x78 */ s32 duration;
    /* 0x7C */ s16 frame;
    /* 0x7E */ s16 clutRow;
    /* 0x80 */ s16 x;
    /* 0x82 */ s16 y;
    /* 0x84 */ union {
        s16 v[2]; /* x, y; 0x1000 = 1.0 */
        s32 both; /* to test the two at once */
    } scale;
    /* 0x88 */ union {
        s16 v[2]; /* x, y */
        s32 both;
    } rot;
    /* 0x8C */ s16 rotZ;
} SpriteAnim;

/* FIGHTSTG_startHitEffect's task: the enemy's view, effect 0x33 on the
   player's fighter, then the script of a hit or a knockout */
typedef struct HitEffect {
    TASK_HEADER(HitEffect);
    /* 0x50 */ struct Models *models;
    /* 0x54 */ struct BattleCamera *camera;
    /* 0x58 */ s32 result; /* 1 a hit, 2 a knockout: the script is result + 1 */
    /* 0x5C */ s32 unk5C; /* FIGHTSTG_startHitEffect's second argument: 1 from WFIGHTMN, 0 from WFIGHTTS; the script's unk74 */
    /* 0x60 */ s32 effectImages; /* FIGHTSTG_findEffectSheet's */
    /* 0x64 */ s32 sheet; /* its file in the high half */
    /* 0x68 */ Vec2 texPos;
} HitEffect;

/* The white flash (FIGHTSTG_startWhiteFlash): fades the screen to white,
   and back once ended */
typedef struct WhiteFlash {
    TASK_HEADER(WhiteFlash);
    /* 0x50 */ s32 speed; /* 0xFF / the frames */
    /* 0x54 */ s32 level; /* added to the screen (white), 0-0xFF */
} WhiteFlash;

/* A partner Digimon's change (FIGHTSTG_startDigimonChange): key1 the new Digimon, key2 the
   side. The stage and two clipped layers wipe the scene away and back. */
typedef struct DigimonChange {
    TASK_HEADER(DigimonChange);
    /* 0x50 */ struct Models *models;
    /* 0x54 */ struct BattleCamera *camera;
    /* 0x58 */ struct FightStage *stage;
    /* 0x5C */ s32 idleMotion; /* the old Digimon's */
    /* 0x60 */ RECT clip0; /* BATTLE_LAYER_MODELS's */
    /* 0x68 */ RECT clip1; /* BATTLE_LAYER_WIREFRAME's */
    /* 0x70 */ s32 file; /* the new Digimon's model's */
} DigimonChange;

typedef struct DigimonChangeChildren {
    /* 0x00 */ struct WhiteFlash *fade;
    /* 0x04 */ Task *unused; /* a child slot nothing fills */
    /* 0x08 */ struct SpriteEffect *effects[4];
} DigimonChangeChildren;

/* A fighter's entrance (FIGHTSTG_startEntrance): key1 its Digimon, key2 its side */
typedef struct Entrance {
    TASK_HEADER(Entrance);
    /* 0x50 */ s32 done;
    /* 0x54 */ struct Models *models;
    /* 0x58 */ struct BattleCamera *camera;
    /* 0x5C */ struct FightStage *stage;
    /* 0x60 */ s32 file; /* its model's */
    /* 0x64 */ s32 weak; /* it comes in with motion 2 (WFIGHTMN_setIdleMotion's weak one) */
} Entrance;

/* A fighter's battle script (FIGHTSTG_updateBattleScript): a list
   of s16 commands in an archive entry of the fighter's file, which play its
   motions, effects, sounds and camera moves */
struct BattleScript {
    TASK_HEADER(BattleScript);
    /* 0x50 */ s32 enemy; /* the script is the enemy's (nonzero) or the partner's */
    /* 0x54 */ s32 index; /* the script, in the fighter's archive */
    /* 0x58 */ s32 hits[4]; /* 0 a hit, 3 a miss; [3] how it ended */
    /* 0x68 */ s32 stage; /* what the stage command's 0x38 stands for */
    /* 0x6C */ s32 effect; /* the sprite effect the effect command's 9999 stands for */
    /* 0x70 */ s32 sound; /* what the sound commands' 0x62 and 0x63 stand for on a hit */
    /* 0x74 */ s32 unk74; /* HitEffect.unk5C; nothing reads it */
    /* 0x78 */ s32 fighter;
    /* 0x7C */ s32 model; /* the Models task's id */
    /* 0x80 */ s32 archive;
    /* 0x84 */ s32 pad; /* nothing uses it */
    /* 0x88 */ Models *models;
    /* 0x8C */ s16 *pc;
    /* 0x90 */ s32 hitsPlayed; /* how many hits command 5 has played */
    /* 0x94 */ s32 soundsPlayed; /* and the sound commands */
    /* 0x98 */ s32 waiting;
    /* 0x9C */ s32 wait; /* the time left */
    /* 0xA0 */ s32 loadStep; /* FIGHTSTG_runScriptEffect's, loading a 2D effect */
    /* 0xA4 */ s32 effectImages; /* FIGHTSTG_findEffectSheet's */
    /* 0xA8 */ s32 effectSheet;
    /* 0xAC */ Vec2 effectTexPos;
};

/* FIGHTSTG_updateHitEffect's children */
typedef struct HitEffectChildren {
    /* 0x0 */ SpriteEffect *effect;
    /* 0x4 */ BattleScript *script;
} HitEffectChildren;

/* The battle script's children */
typedef struct BattleScriptChildren {
    /* 0x00 */ WhiteFlash *fade;
    /* 0x04 */ Jump *jump;
    /* 0x08 */ MoveTask *move;
    /* 0x0C */ BattleSound *sound;
    /* 0x10 */ SpriteEffect *spriteEffects[8];
    /* 0x30 */ EffectModel *effects[3];
    /* 0x3C */ BattleScript *script; /* one it plays */
} BattleScriptChildren;

/* The tasks FIGHTSTG starts for WFIGHTMN and WFIGHTTS */
Entrance *FIGHTSTG_startEntrance(s32 id, s32 side, s32 weak);
/* WFIGHTMN passes the damage as a third argument, which it doesn't read; its
   code loads it, so no parameter list */
HitEffect *FIGHTSTG_startHitEffect(); /* (s32 result, s32 arg1): unprototyped, as WFIGHTMN passes it the damage too */
DigimonChange *FIGHTSTG_startDigimonChange(s32 key1, s32 key2);
ScreenFade *FIGHTSTG_createFader(void);
BattleScript *FIGHTSTG_createBattleScript(void);

/* Shared between the overlay's objects */
s32 FIGHTSTG_getScriptModel(BattleScript *script, s32 type);
void FIGHTSTG_runScriptHits(BattleScript *script, BattleScriptChildren *children);
s32 FIGHTSTG_runScriptModel(BattleScript *script, BattleScriptChildren *children);
s32 FIGHTSTG_runScriptEffect(BattleScript *script, BattleScriptChildren *children);
s32 FIGHTSTG_runScriptEffectModel(BattleScript *script, BattleScriptChildren *children);
void FIGHTSTG_runScriptCamera(BattleScript *script, BattleScriptChildren *children);
s32 FIGHTSTG_runScriptWait(BattleScript *script, BattleScriptChildren *children);
s32 FIGHTSTG_runScriptStage(BattleScript *script, BattleScriptChildren *children);
void FIGHTSTG_runScriptSound(BattleScript *script, BattleScriptChildren *children);
void FIGHTSTG_runScriptFade(BattleScript *script, WhiteFlash **fade);
void FIGHTSTG_updateBattleScript(BattleScript *script, BattleScriptChildren *children);
void FIGHTSTG_updateHitEffect(HitEffect *task, HitEffectChildren *children);
void FIGHTSTG_updateFader(ScreenFade *task);
void FIGHTSTG_startFader(ScreenFade *task, s32 fadeIn, s32 duration);
void FIGHTSTG_drawFader(ScreenFade *task);
void FIGHTSTG_updateDigimonChange(DigimonChange *task, DigimonChangeChildren *children);
void FIGHTSTG_drawWhiteFlash(WhiteFlash *task);
void FIGHTSTG_updateWhiteFlash(WhiteFlash *task);
void FIGHTSTG_updateEntrance(Entrance *task, WhiteFlash **children);
void FIGHTSTG_endWhiteFlash(WhiteFlash *task, s32 frames);
WhiteFlash *FIGHTSTG_startWhiteFlash(s32 frames);
extern s32 FIGHTSTG_battleSounds[]; /* sound ids */
extern EffectSheet FIGHTSTG_effectSheets[];
extern SpriteEffectEntry FIGHTSTG_spriteEffects[];
void FIGHTSTG_nop(void);
void FIGHTSTG_drawSpriteAnim(void *arg, Layer *layer);
void FIGHTSTG_updateSpriteAnim(SpriteAnim *task);
void FIGHTSTG_updateBattleSound(BattleSound *task);
BattleSound *FIGHTSTG_playBattleSound(s32 index, s32 time);
s32 FIGHTSTG_findEffectSheet(s32 effect, s32 *images, s32 *sheet, Vec2 *texPos);
void FIGHTSTG_updateSpriteEffect(SpriteEffect *task, SpriteAnim **children);
SpriteAnim *FIGHTSTG_createSpriteAnim(s16 *data, SVECTOR *pos, s32 sheet, Vec2 *texPos, s32 layerId);
SpriteEffect *FIGHTSTG_startSpriteEffect(s32 effect, const SVECTOR *pos);

#endif /* FIGHTSTG_EFFECT_H */
