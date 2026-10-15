#ifndef FIGHTSTG_MODEL_H
#define FIGHTSTG_MODEL_H

/* FIGHTSTG's models: meshes and their drawing, bones, motions, the models
   task and its controls, faces, effect models, the fighters' data, and the
   jumps and moves of a model (model_mesh.c, models.c, face.c, effect_model.c,
   fighter_info.c, move.c, draw.c and interp.c). */

#include "battle/battle_mode/types.h"

extern MATRIX IDENTITY_MATRIX; /* the root bone's parent (src/engine/data/matrices.c) */

/* A position or rotation, without the SVECTOR pad */
typedef struct ShortVec3 {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
} ShortVec3;

/* Draws one bone of a Model (FIGHTSTG_createMesh), from the parts of an archive */
typedef struct Mesh {
    TASK_HEADER(Mesh);
    /* 0x50 */ s32 noBoundsCheck;
    /* 0x54 */ s32 colorMode; /* Model.setColor's */
    /* 0x58 */ CVECTOR color;
    /* 0x5C */ void *archive;
    /* 0x60 */ ShortVec3 *vertices; /* the count in [0].x, then the vertices */
    /* 0x64 */ ShortVec3 *normals; /* the count in [0].x, then the normals */
    /* 0x68 */ u8 *commands; /* see MeshDrawState */
    /* 0x6C */ ShortVec3 *bounds; /* 9 points (FIGHTSTG_isMeshOnScreen) */
    /* 0x70 */ Vec2 texPos;
    /* 0x78 */ s32 *screen; /* where its vertices land on screen */
    /* 0x7C */ s32 *depth; /* and their depths in the ordering table */
    /* 0x80 */ CVECTOR *colors; /* its normals' colors under the lights */
    /* 0x84 */ MATRIX matrix;
    /* 0xA4 */ void (*draw)(struct Mesh *mesh, s32 layerId, MATRIX *matrix);
    /* 0xA8 */ void (*drawWireframe)(struct Mesh *mesh, s32 layerId, MATRIX *matrix);
} Mesh;

/* A Mesh's drawing state while its drawer (FIGHTSTG_drawMesh) walks its command
   bytes: a high nibble of 8 to 14 sets one of the flags, a low nibble of 1
   the texture page and CLUT, 2 to 5 one of the flat colors, and 0 starts a
   run of polygons, each a 0 byte then its vertices' indices, its normals'
   indices when lit and its UVs when textured. 0xFF ends the commands. */
typedef struct MeshDrawState {
    /* 0x00 */ s32 textured;
    /* 0x04 */ s32 unk4; /* the 0xB nibble: on the disc, 1 on the textured polygons that are not lit and 0 on those that are; nothing reads it */
    /* 0x08 */ s32 unk8; /* the 0xA nibble: on the disc, 0 but in two effect meshes; nothing reads it */
    /* 0x0C */ s32 quad;
    /* 0x10 */ s32 lit;
    /* 0x14 */ s32 gouraud;
    /* 0x18 */ s32 abr; /* the semi-transparency mode + 1, 0 for opaque */
    /* 0x1C */ u8 *cmd;
    /* 0x20 */ s32 *screen;
    /* 0x24 */ s32 *depth;
    /* 0x28 */ u_long *otBase;
    /* 0x2C */ CVECTOR *normalColors;
    /* 0x30 */ Vec2 texPos;
    /* 0x38 */ s32 u;
    /* 0x3C */ s32 v;
    /* 0x40 */ u16 tpage;
    /* 0x42 */ u16 clut;
    /* 0x44 */ CVECTOR color[4];
    /* 0x54 */ union {
        void *ptr;
        u_long *tag; /* the primitive's first word */
        POLY_FT3 *ft3;
        POLY_FT4 *ft4;
        POLY_GT3 *gt3;
        POLY_GT4 *gt4;
        LINE_F2 *lineF2;
        LINE_F4 *lineF4;
    } prim;
    /* 0x58 */ s32 sxy[4]; /* the polygon's screen points */
    /* 0x68 */ u_long *ot;
    /* 0x6C */ u8 uv[4][2];
    /* 0x74 */ CVECTOR colors[4]; /* the polygon's vertex colors */
} MeshDrawState;

/* One part of a Model: a mesh (Mesh) placed by a matrix relative to its
   parent's */
typedef struct ModelBone {
    /* 0x00 */ s32 parent;
    /* 0x04 */ s32 file; /* the mesh's file and index */
    /* 0x08 */ s32 keyFile; /* its keys' file: positions, rotations and scales by frame */
    /* 0x0C */ s32 visible; /* 0 while its scale is tiny */
    /* 0x10 */ SVECTOR pos;
    /* 0x18 */ SVECTOR rot;
    /* 0x20 */ SVECTOR scale;
    /* 0x28 */ MATRIX local;
    /* 0x48 */ MATRIX *parentMatrix;
    /* 0x4C */ MATRIX world;
    /* 0x6C */ SVECTOR prevPos; /* the pose the motion blends from */
    /* 0x74 */ SVECTOR prevRot;
    /* 0x7C */ SVECTOR prevScale;
} ModelBone;

/* What drives a Model, owned by whoever created it */
typedef struct ModelControl {
    /* 0x00 */ s32 active; /* in the Models task's list */
    /* 0x04 */ s32 id; /* the Models task's */
    /* 0x08 */ s32 motion; /* the motion to play */
    /* 0x0C */ s32 restart; /* play it again from the start */
    /* 0x10 */ s32 motionDone;
    /* 0x14 */ s32 fighter; /* the Models task's */
    /* 0x18 */ s32 idleMotion;
    /* 0x1C */ ShortVec3 pos;
    /* 0x22 */ ShortVec3 rot;
    /* 0x28 */ ShortVec3 homePos;
    /* 0x2E */ ShortVec3 homeRot;
    /* 0x34 */ struct {
        s32 enabled;
        s32 layerId; /* the layer its meshes are drawn in */
        s32 wireframe; /* they are drawn as wireframe (Mesh.drawWireframe) */
    } layers[2]; /* the model is drawn once for each one enabled */
} ModelControl;

/* A motion in the motions' archive: its keyframes, step by step */
typedef struct MotionStep {
    /* 0x0 */ s16 index; /* 0x7FFF ends them */
    /* 0x2 */ s16 count; /* frames, 0 for the last one */
    /* 0x4 */ s16 frame;
    /* 0x6 */ s16 endFrame; /* the frame it ends on; 0: it blends into the next one */
} MotionStep;

/*
 * A 3D model (FIGHTSTG_createModel), registered with id 0x11: a tree of bones,
 * each drawn by a Mesh child (children[i] for bone i, children[0] the
 * model's face, FIGHTSTG_createFace), and the motion it plays, from the
 * archive of motions in motionFile.
 */
typedef struct Model {
    TASK_HEADER(Model);
    /* 0x0050 */ s32 boneCount;
    /* 0x0054 */ ModelBone *bones;
    /* 0x0058 */ union {
        SVECTOR v; /* moves the root bone, along its rotation */
        s32 xy; /* vx and vy as one word */
    } move;
    /* 0x0060 */ s32 hasIdle; /* it goes back to its idle motion */
    /* 0x0064 */ ModelControl *control;
    /* 0x0068 */ Vec2 texPos; /* where its textures go in VRAM */
    /* 0x0070 */ s32 texFile;
    /* 0x0074 */ s32 motionFile;
    /* 0x0078 */ s32 motion;
    /* 0x007C */ s32 motionDone;
    /* 0x0080 */ s32 keyframe;
    /* 0x0084 */ s32 frame; /* the pose's */
    /* 0x0088 */ s32 idleFrames[2]; /* where the two idle motions start */
    /* 0x0090 */ s32 blendTarget; /* the frame it blends into, 0 for none */
    /* 0x0094 */ s32 blending;
    /* 0x0098 */ s32 blend; /* how far, in 4096ths */
    /* 0x009C */ s32 toIdle; /* the motion ends in the idle motion */
    /* 0x00A0 */ s32 keyframeCount;
    /* 0x00A4 */ u16 keyframes[0x640];
    /* 0x0D24 */ u16 blendTargets[0x640]; /* the frame a blending keyframe blends into */
    /* 0x19A4 */ u16 blendSources[0x640]; /* and the one it blends from, 0xFFFF for the saved pose */
    /* 0x2624 */ void (*setMotion)(struct Model *model, s32 motion, s32 restart);
    /* 0x2628 */ s32 (*isMotionDone)(struct Model *model);
    /* 0x262C */ void (*setColor)(); /* (model, mode, color): its meshes' */
    /* 0x2630 */ void (*setBoneNoBoundsCheck)();
} Model;

/* A battle effect that is a model (FIGHTSTG_startEffectModel): FIGHTSTG_effectModels lists
   them, ending with id 0 */
typedef struct EffectModelEntry {
    /* 0x0 */ s32 id;
    /* 0x4 */ s32 file; /* the model's file and index */
    /* 0x8 */ s32 motionFile;
} EffectModelEntry;

/* Shows an effect's model at a place until its motion ends
   (FIGHTSTG_startEffectModel) */
typedef struct EffectModel {
    TASK_HEADER(EffectModel);
    /* 0x50 */ s32 effect; /* 0 if it has no entry */
    /* 0x54 */ s32 file;
    /* 0x58 */ s32 motionFile;
    /* 0x5C */ SVECTOR pos;
    /* 0x64 */ SVECTOR rot;
    /* 0x6C */ Vec2 texPos;
    /* 0x74 */ ModelControl control;
} EffectModel;

/* The fighters' file, which FIGHTSTG_fighterCache.funcs reads */
#if VERSION_US || VERSION_JP
#define FILE_FIGHTERS 0x1BE
#elif VERSION_EU
#define FILE_FIGHTERS 0x1CC
#endif

/* FIGHTSTG_interp: the functions that go between values */
typedef struct InterpFuncs {
    /* 0x0 */ void (*nop)(void);
    /* 0x4 */ void (*lerp)(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out); /* t: 0-0x1000 */
    /* 0x8 */ s32 (*ease)(s32 curve, s32 t, s32 value); /* value scaled by a curve of t */
} InterpFuncs;

/* FIGHTSTG_startMove's task: moves a ModelControl to a position */
typedef struct MoveTask {
    TASK_HEADER(MoveTask);
    /* 0x50 */ ModelControl *control;
    /* 0x54 */ ShortVec3 from;
    /* 0x5A */ ShortVec3 to;
    /* 0x60 */ s32 t; /* 0-0x1000 */
    /* 0x64 */ s32 tStep;
} MoveTask;

/* An entry of the fighters' file's list */
typedef struct FighterEntry {
    /* 0x0 */ s16 id;
    /* 0x2 */ u8 index; /* in the partners' or the enemies' table */
    /* 0x3 */ u8 kind; /* 0x3A and up: an enemy */
} FighterEntry;

/* The fighters' file: offsets from its start */
typedef struct FightersFile {
    /* 0x0 */ s32 base; /* the address FighterInfo.face assumes the file is at */
    /* 0x4 */ s32 entries; /* FighterEntry, up to an id 0 */
    /* 0x8 */ s32 partners; /* FighterInfo */
    /* 0xC */ s32 enemies; /* FighterInfoEnemy */
} FightersFile;

/* A fighter's entry in its file, from FIGHTSTG_fighterCache.funcs.getInfo */
typedef struct FighterInfo {
    /* 0x00 */ s32 model;
    /* 0x04 */ s32 motions;
    /* 0x08 */ s32 effects; /* its effect scripts' archive */
    /* 0x0C */ s32 face; /* its FaceRects: an offset in the fighters' file */
    /* 0x10 */ s16 distance; /* how far past 0x1400 from the middle it stands */
    /* 0x12 */ s16 unk12[3]; /* varied on the disc (as 0, 0x700, 0x100); nothing reads it */
    /* 0x18 */ s16 height;
    /* 0x1A */ ShortVec3 camPos[12]; /* the cameras that look at it; an enemy
                                        has 3 of each and then their count */
    /* 0x62 */ ShortVec3 camRef[12];
    /* 0xAA */ s16 camProj[12];
} FighterInfo;

/* An enemy's FighterInfo: it has 3 cameras where a partner has 12.
   FIGHTSTG_fighterCache keeps and returns it as a FighterInfo, so its
   readers cast it back. */
typedef struct FighterInfoEnemy {
    /* 0x00 */ s32 model;
    /* 0x04 */ s32 motions;
    /* 0x08 */ s32 effects;
    /* 0x0C */ s32 face;
    /* 0x10 */ s16 distance;
    /* 0x12 */ s16 unk12[3];
    /* 0x18 */ s16 height;
    /* 0x1A */ ShortVec3 camPos[3];
    /* 0x2C */ ShortVec3 camRef[3];
    /* 0x3E */ s16 camProj[3];
    /* 0x44 */ s16 cameraCount;
} FighterInfoEnemy;

/* The fighters' file (FIGHTSTG_fighterCache.funcs) */
typedef struct FighterInfoFuncs {
    /* 0x0 */ FighterInfo *(*getInfo)(s32 id);
    /* 0x4 */ void (*cacheEntry)(s32 index); /* FIGHTSTG_cacheFighter: the list's entry at index */
    /* 0x8 */ void (*getRange)(u32 enemy, s32 *min, s32 *max); /* the indices of the partners or the enemies */
} FighterInfoFuncs;

/* FIGHTSTG_fighterCache: the last fighter getInfo found, and the fighters'
   file's functions */
typedef struct FighterCache {
    /* 0x00 */ s32 id;
    /* 0x04 */ s32 isEnemy; /* the kind is 0x3A or more */
    /* 0x08 */ s32 index; /* in the partners' or the enemies' table */
    /* 0x0C */ s32 kind;
    /* 0x10 */ FighterInfo *partnerInfo; /* both hold the info found, read by isEnemy */
    /* 0x14 */ FighterInfo *enemyInfo;
    /* 0x18 */ FighterInfoFuncs funcs;
    /* 0x24 */ struct FaceRect *(*getFace)(s32 fighter); /* up to an x of 0xFF */
} FighterCache;

/* The Models task's children: a removed model stays until it is gone */
typedef struct ModelsChildren {
    /* 0x00 */ Model *models[4];
    /* 0x10 */ Model *dying[4];
} ModelsChildren;

/* The fighters' models (FIGHTSTG_createModels), registered as BATTLE_TASK_MODELS: up to four,
   each one with its ModelControl, found by an id */
typedef struct Models {
    TASK_HEADER(Models);
    /* 0x050 */ ModelControl controls[4];
    /* 0x180 */ void (*remove)(struct Models *task, s32 id);
    /* 0x184 */ void (*add)(struct Models *task, s32 id, s32 fighter, s32 visible);
    /* 0x188 */ ModelControl *(*get)(struct Models *task, s32 id);
    /* 0x18C */ void (*setId)(struct Models *task, s32 id, s32 newId);
    /* 0x190 */ s32 (*getFighter)(struct Models *task, s32 id);
    /* 0x194 */ void (*face)(struct Models *task, s32 id); /* ids under 0x10 are one side */
    /* 0x198 */ void (*setIdleMotion)(struct Models *task, s32 id, s32 motion);
} Models;

/* A part of a fighter's face (its eyes, then up to 14 more) and where its
   three frames are in its texture: copied into place by a DR_MOVE */
typedef struct FaceRect {
    /* 0x0 */ u8 x;
    /* 0x1 */ u8 y;
    /* 0x2 */ u8 w; /* 0: unused */
    /* 0x3 */ u8 h;
    /* 0x4 */ u8 frames[3][2];
} FaceRect;

typedef struct FacePart {
    /* 0x00 */ s32 used;
    /* 0x04 */ s32 frame; /* the one drawn */
    /* 0x08 */ FaceRect rect;
} FacePart;

/* A model's face (FIGHTSTG_createFace, the model's first child): the eyes
   blink (or close for some motions) and the other parts loop their frames */
typedef struct Face {
    TASK_HEADER(Face);
    /* 0x050 */ Model *model;
    /* 0x054 */ Vec2 texPos; /* the model's */
    /* 0x05C */ s32 partCount;
    /* 0x060 */ FacePart parts[16];
    /* 0x1A0 */ s32 blinkTimer;
    /* 0x1A4 */ s32 time;
} Face;

/* The jump heights and speeds (t per frame) of the Jump kinds 1-6 */
typedef struct JumpParams {
    /* 0x0 */ s32 height;
    /* 0x4 */ s32 speed;
} JumpParams;

/* A ModelControl's jump (FIGHTSTG_startJump): up and down from its y, or
   for kinds 4 and 5 also forwards from or back to its home */
typedef struct Jump {
    TASK_HEADER(Jump);
    /* 0x50 */ s32 kind;
    /* 0x54 */ s32 distance; /* kind 4: past 0x2800 */
    /* 0x58 */ s32 zTravel; /* kinds 4 and 5: the z it moves from home, negated for the enemy */
    /* 0x5C */ ModelControl *control;
    /* 0x60 */ s32 height;
    /* 0x64 */ s32 y;
    /* 0x68 */ s32 speed;
    /* 0x6C */ s32 t; /* 0-0x1000 */
} Jump;

/* The tasks FIGHTSTG starts for WFIGHTMN and WFIGHTTS */
Models *FIGHTSTG_createModels(void);

/* Shared between the overlay's objects */
extern FighterCache FIGHTSTG_fighterCache;
extern InterpFuncs FIGHTSTG_interp;
void FIGHTSTG_blendBone(Model *model, ModelBone *bone);
void FIGHTSTG_poseBone(Model *model, ModelBone *bone, s32 frame);
void FIGHTSTG_poseBones(Model *model, Mesh **children);
void FIGHTSTG_saveBlendPose(Model *model);
void FIGHTSTG_setMotion(Model *model, s32 motion, s32 restart);
void FIGHTSTG_updateModel(Model *model, Mesh **children);
Mesh *FIGHTSTG_createMesh(void *archive, Vec2 texPos);
void FIGHTSTG_setModelColor(Model *model, s32 mode, CVECTOR *color);
void FIGHTSTG_setBoneNoBoundsCheck(Model *model, s32 bone, s32 value);
s32 FIGHTSTG_isMotionDone(Model *model);
Model *FIGHTSTG_createModel(s32 file, s32 motionFile, Vec2 texPos, ModelControl *control, s32 hasIdle);
Model *FIGHTSTG_createPlainModel(s32 file, s32 motionFile, Vec2 texPos, ModelControl *control);
void FIGHTSTG_lightMesh(Mesh *mesh);
void FIGHTSTG_projectMesh(Mesh *mesh, Layer *layer);
void FIGHTSTG_addMeshPolyGT(MeshDrawState *state);
void FIGHTSTG_addMeshPolyFT(MeshDrawState *state);
s32 FIGHTSTG_isMeshOnScreen(Mesh *mesh, Layer *layer);
void FIGHTSTG_drawMesh(void *arg, Layer *layer);
void FIGHTSTG_drawMeshWireframe(Mesh *mesh, Layer *layer);
void FIGHTSTG_queueMeshDraw(Mesh *mesh, s32 layerId, MATRIX *matrix);
void FIGHTSTG_queueMeshWireframe(Mesh *mesh, s32 layerId, MATRIX *matrix);
void FIGHTSTG_updateMesh(Mesh *mesh);
void FIGHTSTG_updateMove(MoveTask *task);
FighterInfo *FIGHTSTG_getFighterInfo(s32 id);
void FIGHTSTG_cacheFighter(s32 index);
void FIGHTSTG_getFighterRange(u32 enemy, s32 *min, s32 *max);
extern Vec2 FIGHTSTG_fighterTexPos[];
void FIGHTSTG_updateModels(Models *task);
s32 FIGHTSTG_findModelSlot(Models *task, s32 id);
s32 FIGHTSTG_findFreeModelSlot(Models *task);
void FIGHTSTG_removeFighterModel(Models *task, s32 id);
void FIGHTSTG_addFighterModel(Models *task, s32 id, s32 fighter, s32 visible);
ModelControl *FIGHTSTG_getModelControl(Models *task, s32 id);
void FIGHTSTG_setModelId(Models *task, s32 id, s32 newId);
s32 FIGHTSTG_getModelFighter(Models *task, s32 id);
void FIGHTSTG_faceModel(Models *task, s32 id);
void FIGHTSTG_setModelIdleMotion(Models *task, s32 id, s32 motion);
Model *FIGHTSTG_createIdlingModel(s32 file, s32 motionFile, Vec2 texPos, ModelControl *control);
void FIGHTSTG_updateJump(Jump *task);
Jump *FIGHTSTG_startJump(ModelControl *control, s32 kind, s32 distance);
MoveTask *FIGHTSTG_startMove(ModelControl *control, ShortVec3 *to, s32 time);
void FIGHTSTG_drawShadedQuad(s32 layerId, s32 depth, DVECTOR *xy, CVECTOR *colors, s32 semi);
void FIGHTSTG_stepMotion(Model *model);
extern EffectModelEntry FIGHTSTG_effectModels[];
s32 FIGHTSTG_getEyesFrame(Face *task);
void FIGHTSTG_moveFacePart(Face *task, DR_MOVE *prim, s32 part, s32 frame);
void FIGHTSTG_updateFace(Face *task);
Face *FIGHTSTG_createFace(Model *model, s32 fighter);
void FIGHTSTG_projectPoint(Layer *layer, SVECTOR *pos, ShortVec3 *out);
extern JumpParams FIGHTSTG_jumps[];
extern s32 FIGHTSTG_partnerIdleMotion;
void FIGHTSTG_updateEffectModel(EffectModel *task, Model **children);
s32 FIGHTSTG_getEffectModelFile(s32 id);
EffectModel *FIGHTSTG_startEffectModel(s32 id, SVECTOR *pos, SVECTOR *rot);
void FIGHTSTG_interpNop(void);
void FIGHTSTG_lerpVector(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out);
s32 FIGHTSTG_ease(s32 curve, s32 t, s32 value);

/* the functions of FIGHTSTG_battle, FIGHTSTG_action and FIGHTSTG_battleFuncs */
void FIGHTSTG_drawQuad(s32 layerId, s32 depth, DVECTOR *xy, CVECTOR *colors);
void FIGHTSTG_drawBlendedQuad(s32 layerId, s32 depth, DVECTOR *xy, CVECTOR *colors);
FaceRect *FIGHTSTG_getFighterFace(s32 id);

#endif /* FIGHTSTG_MODEL_H */
