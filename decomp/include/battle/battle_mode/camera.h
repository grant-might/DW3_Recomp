#ifndef FIGHTSTG_CAMERA_H
#define FIGHTSTG_CAMERA_H

/* FIGHTSTG's cameras: the fighters' cameras, the shot camera, the battle
   camera and its views, and the camera turns (fighter_camera.c,
   shot_camera.c, battle_camera.c and camera_task.c). */

#include "battle/battle_mode/types.h"
#include "battle/battle_mode/model.h"

/* A fighter's camera (FIGHTSTG_createCamera) on layer 0x1009, from its
   FighterInfo; frames counts the frames it still has to be set */
typedef struct FighterCamera {
    TASK_HEADER(FighterCamera);
    /* 0x50 */ s32 fighter;
    /* 0x54 */ ModelControl *control;
    /* 0x58 */ GsRVIEW2 view;
    /* 0x78 */ s32 proj; /* the projection distance */
    /* 0x7C */ GsCOORDINATE2 coord; /* the view's */
    /* 0xCC */ SVECTOR rot;
    /* 0xD4 */ VECTOR trans;
    /* 0xE4 */ s32 frames;
} FighterCamera;

/* A shot of FIGHTSTG_updateShotCamera's camera */
typedef struct CameraShot {
    /* 0x0 */ s16 time; /* -1 ends the list */
    /* 0x2 */ s16 substate;
} CameraShot;

/* A camera (FIGHTSTG_createShotCamera) that goes through lists of shots
   (FIGHTSTG_cameraShots), the next list picked at random
   (FIGHTSTG_nextShotLists) */
typedef struct ShotCamera {
    TASK_HEADER(ShotCamera);
    /* 0x50 */ struct BattleCamera *camera;
    /* 0x54 */ struct CameraView *view;
    /* 0x58 */ struct Models *models;
    /* 0x5C */ s16 list;
    /* 0x5E */ s16 shot;
    /* 0x60 */ s16 ry; /* where the turn of substate 1 is */
    /* 0x62 */ s16 turned;
    /* 0x64 */ s32 time;
    /* 0x68 */ s32 speed;
} ShotCamera;

/* Where the battle camera looks from and to, as a GsRVIEW2 with the
   transform of its coordinate system */
typedef struct CameraView {
    /* 0x00 */ s32 vpx;
    /* 0x04 */ s32 vpy;
    /* 0x08 */ s32 vpz;
    /* 0x0C */ s32 vrx;
    /* 0x10 */ s32 vry;
    /* 0x14 */ s32 vrz;
    /* 0x18 */ s32 tx;
    /* 0x1C */ s32 ty;
    /* 0x20 */ s32 tz;
    /* 0x24 */ SVECTOR rot;
    /* 0x2C */ s32 rz; /* the roll, in degrees */
    /* 0x30 */ s32 proj; /* the projection distance */
} CameraView;

/* The battle camera (FIGHTSTG_createBattleCamera), registered as BATTLE_TASK_CAMERA on
   a layer: set puts a view, fade goes from one to another in time frames */
typedef struct BattleCamera {
    TASK_HEADER(BattleCamera);
    /* 0x050 */ s32 layerId;
    /* 0x054 */ CameraView current;
    /* 0x088 */ CameraView to;
    /* 0x0BC */ CameraView from;
    /* 0x0F0 */ s32 t; /* 0-0x1000 */
    /* 0x0F4 */ s32 tStep; /* per frame, << 8 */
    /* 0x0F8 */ void (*set)(struct BattleCamera *task, CameraView *view);
    /* 0x0FC */ void (*fade)(struct BattleCamera *task, CameraView *from, CameraView *to, s32 time); /* from: NULL for the current */
    /* 0x100 */ CameraView *(*getEnemyView)(struct BattleCamera *task);
    /* 0x104 */ CameraView *(*getFighterView)(struct BattleCamera *task, s32 id, s32 camera);
} BattleCamera;

BattleCamera *FIGHTSTG_createBattleCamera(s32 layerId);

/* The European version's camera (FIGHTSTG_startCameraShots): one of three lists of shots
   (FIGHTSTG_euCameraShots) picked at random, then back to the enemy's view */
typedef struct CameraShots {
    TASK_HEADER(CameraShots);
    /* 0x50 */ struct BattleCamera *camera;
    /* 0x54 */ struct CameraView *view;
    /* 0x58 */ struct Models *models;
    /* 0x5C */ s16 list;
    /* 0x5E */ s16 shot;
    /* 0x60 */ s16 ry; /* where the turn of substate 6 starts */
    /* 0x62 */ s16 rx;
    /* 0x64 */ s32 pad64; /* nothing uses it */
    /* 0x68 */ s32 time;
    /* 0x6C */ s32 pad6C; /* nothing uses it */
    /* 0x70 */ struct CameraView to; /* where substates 4 and 5 fade to */
} CameraShots;

/* The camera's turn around the fighters when the player loses
   (FIGHTSTG_updateCameraTurn) */
typedef struct CameraTurn {
    TASK_HEADER(CameraTurn);
    /* 0x50 */ BattleCamera *camera;
} CameraTurn;

/* The tasks FIGHTSTG starts for WFIGHTMN and WFIGHTTS */
CameraTurn *FIGHTSTG_startCameraTurn(void);
CameraShots *FIGHTSTG_startCameraShots(void);

/* Shared between the overlay's objects */
void FIGHTSTG_updateShotCamera(ShotCamera *task);
ShotCamera *FIGHTSTG_createShotCamera(void);
void FIGHTSTG_updateCameraTurn(CameraTurn *task, BattleScript **children);
void FIGHTSTG_updateCameraShots(CameraShots *task);
void FIGHTSTG_applyCamera(FighterCamera *task);
void FIGHTSTG_loadCamera(FighterCamera *task);
void FIGHTSTG_updateCamera(FighterCamera *task);
FighterCamera *FIGHTSTG_createCamera(s32 fighter, ModelControl *control);
void FIGHTSTG_updateBattleCamera(BattleCamera *task);
void FIGHTSTG_setBattleCameraView(BattleCamera *task, CameraView *view);
void FIGHTSTG_fadeBattleCamera(BattleCamera *task, CameraView *from, CameraView *to, s32 time);
CameraView *FIGHTSTG_getFighterView(BattleCamera *task, s32 id, s32 camera);
CameraView *FIGHTSTG_getEnemyView(BattleCamera *task);
extern CameraView FIGHTSTG_fighterView;
extern CameraShot FIGHTSTG_cameraShots[4][6];
extern u8 FIGHTSTG_nextShotLists[8][3];
#if VERSION_EU || VERSION_JP
extern CameraShot FIGHTSTG_euCameraShots[3][3];
#endif

#endif /* FIGHTSTG_CAMERA_H */
