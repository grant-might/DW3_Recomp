#ifndef FIGHTSTG_STAGE_H
#define FIGHTSTG_STAGE_H

/* FIGHTSTG's fight stages and their lights (stage.c and lights.c). */

#include "battle/battle_mode/types.h"
#include "battle/battle_mode/model.h"

/* The fight stages, the battle's backgrounds: WFIGHTTS lists them as
   MFSTG001-027 */
#if VERSION_US || VERSION_JP
#define FILE_FIGHT_STAGES 0x1BD
#elif VERSION_EU
#define FILE_FIGHT_STAGES 0x1CB
#endif

/* A fight stage's lights: three flat lights and the ambient colour */
typedef struct LightSet {
    /* 0x00 */ GsF_LIGHT lights[3];
    /* 0x30 */ s32 ambient[3];
} LightSet;

/* A fight stage, in FILE_FIGHT_STAGES */
typedef struct FightStageInfo {
    /* 0x00 */ s32 model; /* file << 16 | index */
    /* 0x04 */ s32 motions;
    /* 0x08 */ s32 music; /* an index in FIGHTSTG_stageMusic, or -1 */
    /* 0x0C */ u8 bgColor[3];
    /* 0x0F */ u8 padF; /* 0 in every stage; nothing uses it */
    /* 0x10 */ u8 noBoundsBones[8]; /* bones given to the model's setBoneNoBoundsCheck, up to a 0 */
    /* 0x18 */ LightSet lights;
    /* 0x54 */ s32 pad54; /* 0 in every stage; nothing uses it */
} FightStageInfo;

/* The fight stage (FIGHTSTG_createStage), registered as BATTLE_TASK_STAGE: its model,
   which fades in from black, and its music. setStage fades it out and
   the new one in. */
typedef struct FightStage {
    TASK_HEADER(FightStage);
    /* 0x50 */ s32 stage;
    /* 0x54 */ s32 prevStage;
    /* 0x58 */ ModelControl control;
    /* 0xA4 */ s32 fade; /* 0-0x1000 */
    /* 0xA8 */ s32 fadeStep;
    /* 0xAC */ s32 fadeInTime;
    /* 0xB0 */ s32 fadeOutTime;
    /* 0xB4 */ SVECTOR colorFrom; /* the model's */
    /* 0xBC */ SVECTOR colorTo;
    /* 0xC4 */ SVECTOR color;
    /* 0xCC */ SVECTOR bgFrom; /* SCREEN_LAYER's background */
    /* 0xD4 */ SVECTOR bgTo;
    /* 0xDC */ SVECTOR bg;
    /* 0xE4 */ s16 voice; /* the music's */
    /* 0xE8 */ void (*setStage)(struct FightStage *task, s32 stage, s32 fadeOutTime, s32 fadeInTime);
} FightStage;

/* The stage lights (FIGHTSTG_createLights), registered as BATTLE_TASK_LIGHTS: set puts a
   light set, fade goes from one to another in time frames */
typedef struct Lights {
    TASK_HEADER(Lights);
    /* 0x050 */ s32 layerId;
    /* 0x054 */ LightSet current;
    /* 0x090 */ LightSet to;
    /* 0x0CC */ LightSet from;
    /* 0x108 */ s32 t; /* 0-0x1000 */
    /* 0x10C */ s32 tStep; /* per frame */
    /* 0x110 */ void (*set)(struct Lights *task, LightSet *set);
    /* 0x114 */ void (*fade)(struct Lights *task, LightSet *from, LightSet *to, s32 time); /* from: NULL for the current */
    /* 0x118 */ LightSet *(*getStageLights)(struct Lights *task, s32 stage);
} Lights;

/* The tasks FIGHTSTG starts for WFIGHTMN and WFIGHTTS */
FightStage *FIGHTSTG_createStage(s32 id, s32 fadeInTime);
Lights *FIGHTSTG_createLights(s32 layerId);

/* Shared between the overlay's objects */
extern s32 FIGHTSTG_stageMusic[];
void FIGHTSTG_fadeStage(FightStage *task, Model **children);
void FIGHTSTG_updateStage(FightStage *task, Model **children);
void FIGHTSTG_setStage(FightStage *task, s32 id, s32 fadeOutTime, s32 fadeInTime);
void FIGHTSTG_updateLights(Lights *task);
void FIGHTSTG_setLights(Lights *task, LightSet *set);
void FIGHTSTG_fadeLights(Lights *task, LightSet *from, LightSet *to, s32 time);
LightSet *FIGHTSTG_getStageLights(Lights *task, s32 stage);
s32 FIGHTSTG_randomStage(void);
s32 FIGHTSTG_getStageMotionsFile(s32 id);

#endif /* FIGHTSTG_STAGE_H */
