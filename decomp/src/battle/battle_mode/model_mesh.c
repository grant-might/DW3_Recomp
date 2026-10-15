/* The first object of FIGHTSTG.PRO, the models and their meshes.
   FIGHTSTG.PRO was seven objects: each one's jump tables are aligned to 8
   from the start of its own rodata, and they are 4 bytes past a multiple of
   8 from 0x80082464 (USA) to 0x80082480, from 0x800825B4 to 0x800825D0 and
   from 0x8008267C on. Where each object's code starts is only known to be
   between the function with the last jump table of the object before and
   the one with its first; the last object (camera_task.c) differs between
   the versions. The second object is stage.c; the third to the sixth are
   cut into modules, in their link order (entrance.c to fader.c, lights.c
   and face.c, battle_script.c and first_tech.c, item.c to interp.c), each
   with its own rodata and data; a cut is only where each jump table keeps
   its place. Each object's or module's data is in its own file, in address
   order; the zeros that end the file, after the last object's data, are
   here. */

#include "battle/battle_mode.h"
#include "engine/gte.h"

/* Blends a bone's position, rotation and scale from the saved pose
   (FIGHTSTG_saveBlendPose) towards their first keys at or after the model's
   frame, by its blend; when either scale has a 0, the key's is taken as it
   is */
void FIGHTSTG_blendBone(Model *model, ModelBone *bone) {
    s32 i;
    void *archive;
    SVECTOR *key;
    SVECTOR *out;
    SVECTOR cur;
    SVECTOR prev;
    SVECTOR diff;
    SVECTOR step;

    archive = FILE_CACHE.getEntry(bone->keyFile);
    for (i = 0; i < 9; i += 3) {
        key = FILE_CACHE.getArchiveEntry(i / 3, archive);
        switch (i) {
        case 3:
            out = &bone->rot;
            break;
        case 6:
            out = &bone->scale;
            break;
        case 0:
        default:
            out = &bone->pos;
            break;
        }
        for (; key->pad < model->frame; key++) {
        }
        cur = *key;
        switch (i) {
        case 0:
        default:
            prev = bone->prevPos;
            break;
        case 3:
            prev = bone->prevRot;
            break;
        case 6:
            prev = bone->prevScale;
            break;
        }
        if (i == 6 && (prev.vx == 0 || prev.vy == 0 || prev.vz == 0 || cur.vx == 0 || cur.vy == 0 || cur.vz == 0)) {
            *out = cur;
            return;
        }
        gte_lddp(model->blend);
        diff.vx = cur.vx - prev.vx;
        diff.vy = cur.vy - prev.vy;
        diff.vz = cur.vz - prev.vz;
        gte_ldsv(&diff);
        gte_gpf12();
        *out = prev;
        gte_stsv(&step);
        out->vx += step.vx;
        out->vy += step.vy;
        out->vz += step.vz;
    }
}

/* Sets a bone's position, rotation and scale to their keys at FRAME,
   interpolated between the keys around it */
void FIGHTSTG_poseBone(Model *model, ModelBone *bone, s32 frame) {
    s32 i;
    void *archive;
    SVECTOR *key;
    SVECTOR *out;
    s32 found;
    s32 span;
    SVECTOR cur;
    SVECTOR prev;
    SVECTOR diff;
    SVECTOR step;

    archive = FILE_CACHE.getEntry(bone->keyFile);
    for (i = 0; i < 9; i += 3) {
        key = FILE_CACHE.getArchiveEntry(i / 3, archive);
        found = 0;
        switch (i) {
        case 3:
            out = &bone->rot;
            break;
        case 6:
            out = &bone->scale;
            break;
        case 0:
        default:
            out = &bone->pos;
            break;
        }
        for (;; key++) {
            if (key->pad == frame) {
                found = 1;
                break;
            }
            if (key->pad >= frame) {
                break;
            }
        }
        if (found) {
            *out = *key;
        } else {
            cur = key[0];
            prev = key[-1];
            span = cur.pad - prev.pad;
            gte_lddp(((frame - prev.pad) << 12) / span);
            diff.vx = cur.vx - prev.vx;
            diff.vy = cur.vy - prev.vy;
            diff.vz = cur.vz - prev.vz;
            gte_ldsv(&diff);
            gte_gpf12();
            *out = prev;
            gte_stsv(&step);
            out->vx += step.vx;
            out->vy += step.vy;
            out->vz += step.vz;
        }
    }
}

/* Moves the root bone by the model's move along its rotation, then poses
   (FIGHTSTG_poseBone) or blends (FIGHTSTG_blendBone) each other bone */
void FIGHTSTG_poseBones(Model *model, Mesh **children) {
    ModelBone *bone = model->bones;
    VECTOR moved;
    s32 i;

    if (model->move.xy != 0 || model->move.v.vz != 0) {
        gte_SetRotMatrix(&bone->local);
        gte_ldv0(&model->move.v);
        gte_rtv0();
        gte_stlvnl(&moved);
        model->bones->pos.vx += moved.vx;
        model->bones->pos.vy += moved.vy;
        model->bones->pos.vz += moved.vz;
        model->move.v.vz = 0;
        model->move.v.vy = 0;
        model->move.v.vx = 0;
    }
    bone = model->bones;
    if (model->blending == 0) {
        for (i = 1, bone++; i < model->boneCount; i++, bone++) {
            FIGHTSTG_poseBone(model, bone, model->frame);
        }
    } else {
        for (i = 1, bone++; i < model->boneCount; i++, bone++) {
            FIGHTSTG_blendBone(model, bone);
        }
    }
}

/* Saves each bone's pose as the one a blend starts from */
void FIGHTSTG_saveBlendPose(Model *model) {
    ModelBone *bone = model->bones;
    s32 i;

    for (i = 0; i < model->boneCount; i++, bone++) {
        bone->prevPos = bone->pos;
        bone->prevRot = bone->rot;
        bone->prevScale = bone->scale;
    }
}

/* Model.setMotion: lays out the motion's keyframes */
void FIGHTSTG_setMotion(Model *model, s32 motion, s32 restart) {
    MotionStep *step;
    s32 count;
    s32 n;
    s32 i;
    s32 to;
    s32 from;

    if (restart != 1 && model->motion == motion) {
        return;
    }
    model->motion = motion;
    model->keyframe = 1;
    model->motionDone = 0;
    model->toIdle = 0;
    step = FILE_CACHE.getArchiveEntry(motion - 1, FILE_CACHE.getEntry(model->motionFile));
    count = 0;
    while (step->index != 0x7FFF) {
        n = step->count;
        if (n == 0) {
            model->keyframes[count] = step->frame;
            model->blendTargets[count] = step->endFrame;
            count++;
            break;
        }
        if (step->endFrame == 0) {
            to = step[1].frame;
            if (to == -1) {
                to = model->idleFrames[model->control->idleMotion];
                model->toIdle = 1;
            }
            if (step->index != 0) {
                from = step[-1].endFrame;
            } else {
                from = 0xFFFF;
            }
            for (i = 0; i < n; i++) {
                model->blendSources[count] = from;
                model->blendTargets[count] = to;
                model->keyframes[count] = (((i + 1) << 12) / (n + 1)) | 0x8000;
                count++;
            }
        } else {
            for (i = 0; i < n; i++) {
                model->keyframes[count] = step->frame + i;
                model->blendTargets[count] = 0;
                count++;
            }
        }
        step++;
    }
    model->keyframeCount = count;
    model->keyframe = 1;
}

/* Steps a model's motion by the frames gone by, blending into a new pose
   where a keyframe has 0x8000, and starts the idle motion when it ends */
void FIGHTSTG_stepMotion(Model *model) {
    s32 key = model->keyframe;
    ModelBone *bone;
    s32 i;

    if (model->motionDone == 0) {
        if (model->keyframes[key] & 0x8000) {
            if (model->blendTarget != model->blendTargets[key]) {
                model->blendTarget = model->blendTargets[key];
                if (model->blendSources[key] != 0xFFFF) {
                    bone = model->bones;
                    for (i = 1, bone++; i < model->boneCount; i++, bone++) {
                        FIGHTSTG_poseBone(model, bone, model->blendSources[key]);
                    }
                }
                FIGHTSTG_saveBlendPose(model);
            }
            model->frame = model->blendTargets[key];
            model->blend = model->keyframes[key] & 0x7FFF;
            model->blending = 1;
        } else {
            model->blendTarget = 0;
            model->frame = model->keyframes[key];
            model->blending = 0;
        }
        model->keyframe += FIGHTSTG_battle.frames;
        if (model->keyframe >= model->keyframeCount) {
            model->keyframe = model->keyframeCount - 1;
        }
        key = model->keyframe;
        switch (model->keyframes[key]) {
        case 0x8000:
            model->keyframe = model->blendTargets[key];
            break;
        case 0xFFFF:
            model->motionDone = 1;
            model->control->motionDone = 1;
            break;
        }
    } else {
        model->blendTarget = 0;
        model->blending = 0;
        if (model->hasIdle != 0 && model->toIdle != 0) {
            model->control->motion = model->control->idleMotion + 1;
            FIGHTSTG_setMotion(model, model->control->idleMotion + 1, 0);
        }
    }
}

/* The model's task: loads its textures and makes its meshes and face, then
   each frame follows its control's motion, steps and poses it, places the
   root at the control's position, builds each bone's matrices (a bone scaled
   to 48 or less is hidden) and queues its meshes on the control's layers;
   frees the bones when killed */
void FIGHTSTG_updateModel(Model *model, Mesh **children) {
    TimLoader loader;
    VECTOR scale;
    ModelBone *bone;
    void *archive;
    MotionStep *idle;
    s32 i;
    s32 j;
    ModelBone *drawn;
    s32 b;
    ModelBone *linked;
    s32 c;
    s32 d;

    switch (model->state) {
    case TASK_INIT:
    default:
        if (model->texFile != 0) {
            initTimLoader(&loader);
            loader.setImagePos(model->texPos.x, model->texPos.y);
            loader.loadArchive(FILE_CACHE.getEntry(model->texFile));
        }
        for (b = 0, linked = model->bones; b < model->boneCount; b++, linked++) {
            if (b != 0) {
                linked->parentMatrix = &model->bones[linked->parent].world;
            }
        }
        for (c = 0; c < model->boneCount; c++) {
            if (c != 0) {
                children[c + 1] = FIGHTSTG_createMesh(FILE_CACHE.getEntry(model->bones[c].file), model->texPos);
            }
        }
        /* child 0 is the face, the others the bones' meshes */
        children[0] = (Mesh *)FIGHTSTG_createFace(model, model->control->fighter);
        model->motion = 1;
        if (model->hasIdle != 0) {
            archive = FILE_CACHE.getEntry(model->motionFile);
            /* where the first step of each idle motion starts */
            idle = FILE_CACHE.getArchiveEntry(0, archive);
            model->idleFrames[0] = idle->frame;
            idle = FILE_CACHE.getArchiveEntry(1, archive);
            model->idleFrames[1] = idle->frame;
        }
        FIGHTSTG_setMotion(model, model->control->idleMotion + 1, 1);
        model->control->motion = model->control->idleMotion + 1;
        model->nextState(model);
        break;
    case TASK_RUN:
        if (model->control->restart != 0 || model->motion != model->control->motion) {
            model->control->restart = 0;
            FIGHTSTG_setMotion(model, model->control->motion, 1);
            switch (model->control->motion) {
            case 1:
                model->control->idleMotion = 0;
                break;
            case 2:
                model->control->idleMotion = 1;
                break;
            }
        }
        if (model->boneCount == 0) {
            break;
        }
        FIGHTSTG_stepMotion(model);
        FIGHTSTG_poseBones(model, children);
        model->bones[0].pos.vx = model->control->pos.x;
        model->bones[0].pos.vy = model->control->pos.y;
        model->bones[0].pos.vz = model->control->pos.z;
        model->bones[0].rot.vx = model->control->rot.x;
        model->bones[0].rot.vy = model->control->rot.y;
        model->bones[0].rot.vz = model->control->rot.z;
        for (i = 0, bone = model->bones; i < model->boneCount; i++, bone++) {
            scale.vx = bone->scale.vx;
            scale.vy = bone->scale.vy;
            scale.vz = bone->scale.vz;
            if ((scale.vx | scale.vy | scale.vz) <= 48) {
                bone->visible = 0;
            } else {
                bone->visible = 1;
                RotMatrixZYX_gte(&bone->rot, &bone->local);
                ScaleMatrix(&bone->local, &scale);
                bone->local.t[0] = bone->pos.vx;
                bone->local.t[1] = bone->pos.vy;
                bone->local.t[2] = bone->pos.vz;
                gte_CompMatrix(bone->parentMatrix, &bone->local, &bone->world);
            }
        }
        for (j = 0; j < 2; j++) {
            if (model->control->layers[j].enabled) {
                for (d = 0, drawn = model->bones; d < model->boneCount; d++, drawn++) {
                    if (d != 0 && drawn->visible) {
                        if (model->control->layers[j].wireframe) {
                            children[d + 1]->drawWireframe(children[d + 1], model->control->layers[j].layerId, &drawn->world);
                        } else {
                            children[d + 1]->draw(children[d + 1], model->control->layers[j].layerId, &drawn->world);
                        }
                    }
                }
            }
        }
        break;
    case TASK_DONE:
        break;
    case TASK_KILL:
        if (model->bones != NULL) {
            HEAP.free(model->bones);
        }
        break;
    }
}

/* Model.setColor: sets the color mode, and the color unless COLOR is NULL, of
   the meshes of each bone but the root (once for each of the control's
   enabled layers) */
void FIGHTSTG_setModelColor(Model *model, s32 mode, CVECTOR *color) {
    Mesh **children = model->children;
    s32 i;
    s32 j;

    for (i = 0; i < 2; i++) {
        if (model->control->layers[i].enabled) {
            for (j = 0; j < model->boneCount; j++) {
                if (j != 0) {
                    children[j + 1]->colorMode = mode;
                    if (color != NULL) {
                        children[j + 1]->color = *color;
                    }
                }
            }
        }
    }
}

/* Model.setBoneNoBoundsCheck: sets bone's Mesh noBoundsCheck when either of the
   control's layers is enabled and bone isn't 0. The match
   depends on the pointer to the bone's slot, which children[bone + 1]
   computes with the addu's operands the other way round. */
void FIGHTSTG_setBoneNoBoundsCheck(Model *model, s32 bone, s32 value) {
    Mesh **meshes = (Mesh **)model->children + bone;
    s32 i;

    for (i = 0; i < 2; i++) {
        if (model->control->layers[i].enabled && bone != 0) {
            meshes[1]->noBoundsCheck = value;
        }
    }
}

/* Model.isMotionDone */
s32 FIGHTSTG_isMotionDone(Model *model) {
    return model->motionDone;
}

/* Creates a model (registered as BATTLE_TASK_MODEL) from the bone list in
   FILE: each bone's parent, mesh file and keys file, with the file's high
   half; HASIDLE makes its motions go back to its idle motion */
Model *FIGHTSTG_createModel(s32 file, s32 motionFile, Vec2 texPos, ModelControl *control, s32 hasIdle) {
    s32 high = file & 0xFFFF0000;
    s32 *entry = FILE_CACHE.getEntry(file);
    s32 count = entry[1] + 1;
    Model *model = createTaskWithId(FIGHTSTG_updateModel, sizeof(Model), (entry[1] + 2) * 4, BATTLE_TASK_MODEL);
    s32 i;

    model->bones = HEAP.alloc(count * sizeof(ModelBone), 2);
    model->control = control;
    model->hasIdle = hasIdle;
    model->texPos = texPos;
    model->texFile = *entry++;
    if (model->texFile != 0) {
        model->texFile |= high;
    }
    model->boneCount = count;
    model->bones[0].parent = 0;
    model->bones[0].file = 0;
    model->bones[0].keyFile = 0;
    model->bones[0].parentMatrix = &IDENTITY_MATRIX;
    model->bones[0].pos.vx = 0;
    model->bones[0].pos.vy = 0;
    model->bones[0].pos.vz = 0;
    model->bones[0].rot.vx = 0;
    model->bones[0].rot.vy = 0;
    model->bones[0].rot.vz = 0;
    model->bones[0].scale.vx = 0x1000;
    model->bones[0].scale.vy = 0x1000;
    model->bones[0].scale.vz = 0x1000;
    entry++;
    for (i = 1; i < count; i++) {
        model->bones[i].parent = *entry++;
        model->bones[i].file = *entry++ | high;
        model->bones[i].keyFile = *entry++ | high;
    }
    model->setMotion = FIGHTSTG_setMotion;
    model->isMotionDone = FIGHTSTG_isMotionDone;
    model->setColor = FIGHTSTG_setModelColor;
    model->motionFile = motionFile;
    model->setBoneNoBoundsCheck = FIGHTSTG_setBoneNoBoundsCheck;
    return model;
}

/* A model whose motions go back to its idle motion (FIGHTSTG_createModel):
   the fighters' */
Model *FIGHTSTG_createIdlingModel(s32 file, s32 motionFile, Vec2 texPos, ModelControl *control) {
    return FIGHTSTG_createModel(file, motionFile, texPos, control, 1);
}

/* A model whose motions just end (FIGHTSTG_createModel) */
Model *FIGHTSTG_createPlainModel(s32 file, s32 motionFile, Vec2 texPos, ModelControl *control) {
    return FIGHTSTG_createModel(file, motionFile, texPos, control, 0);
}

/* The colors of a Mesh's normals under the lights, through its matrix */
void FIGHTSTG_lightMesh(Mesh *mesh) {
    ShortVec3 *normal = mesh->normals;
    s32 count = normal->x;
    MATRIX light;
    CVECTOR *color;
    s32 i;

    normal++;
    if (count == 0) {
        return;
    }
    gte_CompMatrix(&GsLIGHTWSMATRIX, &mesh->matrix, &light);
    gte_SetLightMatrix(&light);
    if (mesh->colors == NULL) {
        mesh->colors = HEAP.alloc(count * 4, 2);
    }
    color = mesh->colors;
    gte_ldv0_unaligned(normal);
    gte_ncs();
    gte_strgb(color);
    i = 1;
    normal++;
    while (i < count) {
        gte_ldv0_unaligned(normal);
        gte_ncs();
        normal++;
        color++;
        i++;
        gte_strgb(color);
    }
}

/* The screen positions of a Mesh's vertices and their depths in the layer's
   ordering table */
void FIGHTSTG_projectMesh(Mesh *mesh, Layer *layer) {
    ShortVec3 *vertex = mesh->vertices;
    s32 shift = 14 - layer->getOtShift(layer);
    s32 count = vertex->x;
    s32 *screen;
    s32 *depth;
    s32 i;
    s32 z;

    vertex++;
    if (mesh->screen == NULL) {
        mesh->screen = HEAP.alloc(count * 4, 2);
    }
    screen = mesh->screen;
    if (mesh->depth == NULL) {
        mesh->depth = HEAP.alloc(count * 4, 2);
    }
    depth = mesh->depth;
    gte_ldv0_unaligned(vertex);
    gte_rtps();
    gte_stsxy(screen);
    gte_stszotz(&z);
    i = 1;
    vertex++;
    while (i < count) {
        gte_ldv0_unaligned(vertex);
        gte_rtps();
        vertex++;
        screen++;
        i++;
        *depth++ = z >> shift;
        gte_stsxy(screen);
        gte_stszotz(&z);
    }
    *depth = z >> shift;
}

/* Adds the polygon in state as a gouraud-shaded textured triangle or quad;
   the colors, points and UVs are copied a word or a halfword at a time. */
void FIGHTSTG_addMeshPolyGT(MeshDrawState *state) {
    *(CVECTOR *)&state->prim.gt4->r0 = state->colors[0];
    *(CVECTOR *)&state->prim.gt4->r1 = state->colors[1];
    *(CVECTOR *)&state->prim.gt4->r2 = state->colors[2];
    if (state->quad) {
        *(CVECTOR *)&state->prim.gt4->r3 = state->colors[3];
    }
    *(s32 *)&state->prim.gt4->x0 = state->sxy[0];
    *(s32 *)&state->prim.gt4->x1 = state->sxy[1];
    *(s32 *)&state->prim.gt4->x2 = state->sxy[2];
    *(u16 *)&state->prim.gt4->u0 = *(u16 *)state->uv[0];
    *(u16 *)&state->prim.gt4->u1 = *(u16 *)state->uv[1];
    *(u16 *)&state->prim.gt4->u2 = *(u16 *)state->uv[2];
    state->prim.gt4->clut = state->clut;
    state->prim.gt4->tpage = state->tpage;
    if (state->quad) {
        setPolyGT4(state->prim.gt4);
        if (state->abr) {
            setSemiTrans(state->prim.gt4, 1);
        }
        *(s32 *)&state->prim.gt4->x3 = state->sxy[3];
        *(u16 *)&state->prim.gt4->u3 = *(u16 *)state->uv[3];
        addPrim(state->ot, state->prim.gt4);
        state->prim.gt4++;
    } else {
        setPolyGT3(state->prim.gt3);
        if (state->abr) {
            setSemiTrans(state->prim.gt3, 1);
        }
        addPrim(state->ot, state->prim.gt3);
        state->prim.gt3++;
    }
}

/* The same as a flat-shaded textured triangle or quad */
void FIGHTSTG_addMeshPolyFT(MeshDrawState *state) {
    *(CVECTOR *)&state->prim.ft4->r0 = state->colors[0];
    *(s32 *)&state->prim.ft4->x0 = state->sxy[0];
    *(s32 *)&state->prim.ft4->x1 = state->sxy[1];
    *(s32 *)&state->prim.ft4->x2 = state->sxy[2];
    *(u16 *)&state->prim.ft4->u0 = *(u16 *)state->uv[0];
    *(u16 *)&state->prim.ft4->u1 = *(u16 *)state->uv[1];
    *(u16 *)&state->prim.ft4->u2 = *(u16 *)state->uv[2];
    state->prim.ft4->clut = state->clut;
    state->prim.ft4->tpage = state->tpage;
    if (state->quad) {
        setPolyFT4(state->prim.ft4);
        if (state->abr) {
            setSemiTrans(state->prim.ft4, 1);
        }
        *(s32 *)&state->prim.ft4->x3 = state->sxy[3];
        *(u16 *)&state->prim.ft4->u3 = *(u16 *)state->uv[3];
        addPrim(state->ot, state->prim.ft4);
        state->prim.ft4++;
    } else {
        setPolyFT3(state->prim.ft3);
        if (state->abr) {
            setSemiTrans(state->prim.ft3, 1);
        }
        addPrim(state->ot, state->prim.ft3);
        state->prim.ft3++;
    }
}

/* Whether a Mesh may be on screen: the screen positions of the 9 points of
   its bounds against the layer's clip, 64 pixels bigger each way;
   noBoundsCheck skips the check. The match depends on the + 128s kept in w
   and h, which gcc otherwise folds into the - 64s. */
s32 FIGHTSTG_isMeshOnScreen(Mesh *mesh, Layer *layer) {
    ShortVec3 *corner;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    s32 i;
    DVECTOR screen;
    s32 flag;

    if (mesh->noBoundsCheck != 0) {
        return 1;
    }
    corner = mesh->bounds;
    left = 0;
    right = 0;
    top = 0;
    bottom = 0;
    for (i = 0; i < 9; ) {
        gte_ldv0_unaligned(corner);
        gte_rtps();
        if (i == 0) {
            x = layer->env.clip.x - 64;
            left = x - layer->offsetX;
            w = layer->env.clip.w + 128;
            right = x + w - layer->offsetX;
            y = layer->env.clip.y - 64;
            top = y - layer->offsetY;
            h = layer->env.clip.h + 128;
            bottom = y + h - layer->offsetY;
        }
        i++;
        corner++;
        gte_stsxy(&screen);
        gte_stflg(&flag);
        if (screen.vx >= left && screen.vx <= right && screen.vy >= top && screen.vy <= bottom) {
            return 1;
        }
    }
    return 0;
}

/* Sets the texture page, the CLUT and the UV offset from a command's bytes */
static inline void setMeshTexture(MeshDrawState *state) {
    s32 x, y;
    s32 cx, cy, tp;

    x = state->texPos.x;
    y = state->texPos.y;
    state->u = state->cmd[1] + (state->cmd[2] << 8);
    state->v = state->cmd[3];
    cx = state->cmd[4];
    cx += x;
    cy = state->cmd[5] + y;
    tp = state->cmd[6];
    state->clut = getClut(cx, cy);
    state->tpage = getTPage(tp, state->abr ? state->abr - 1 : 0, x + (state->cmd[2] << 6), y);
    state->cmd += 7;
}

/* Draws a run of polygons: adds each one that faces the camera to the
   ordering table, then steps past its indices and UVs */
static inline void drawMeshPolygons(MeshDrawState *state) {
    s32 opz;
    s32 otz;
    s32 i0, i1, i2, i3;
    s32 z1, z2;
    s32 n;
    s32 k;
    u8 *p;
    u8 *q;

    do {
        state->cmd++;
        i0 = state->cmd[0];
        i1 = state->cmd[1];
        i2 = state->cmd[2];
        i3 = 0;
        if (state->quad) {
            i3 = state->cmd[3];
        }
        state->sxy[0] = state->screen[i0];
        state->sxy[1] = state->screen[i1];
        state->sxy[2] = state->screen[i2];
        if (state->quad) {
            state->sxy[3] = state->screen[i3];
        }
        /* the checks and drawing are a do-while with breaks: its loop
           notes weigh their references 4 instead of 3, so the screen
           points outrank the screen base and the depth base outranks z1 */
        do {
            gte_ldsxy3(state->sxy[0], state->sxy[1], state->sxy[2]);
            gte_nclip();
            if (state->sxy[0] == state->sxy[1] || state->sxy[0] == state->sxy[2] ||
                state->sxy[1] == state->sxy[2]) {
                break;
            }
            if (state->quad && (state->sxy[0] == state->sxy[3] || state->sxy[1] == state->sxy[3] ||
                               state->sxy[2] == state->sxy[3])) {
                break;
            }
            gte_stopz(&opz);
            if (opz <= 0) {
                break;
            }
            if (state->lit) {
                p = state->cmd + (state->quad + 3);
                state->colors[0] = state->normalColors[p[0]];
                state->colors[1] = state->normalColors[p[1]];
                state->colors[2] = state->normalColors[p[2]];
                if (state->quad) {
                    state->colors[3] = state->normalColors[p[3]];
                }
            }
            if (state->textured) {
                k = state->quad + 3;
                q = state->cmd + (state->lit ? k + (state->quad + 3) : k);
                state->uv[0][0] = q[0] + state->u;
                state->uv[0][1] = q[1] + state->v;
                state->uv[1][0] = q[2] + state->u;
                state->uv[1][1] = q[3] + state->v;
                state->uv[2][0] = q[4] + state->u;
                state->uv[2][1] = q[5] + state->v;
                if (state->quad) {
                    state->uv[3][0] = q[6] + state->u;
                    state->uv[3][1] = q[7] + state->v;
                }
            }
            otz = state->depth[i0];
            z1 = state->depth[i1];
            z2 = state->depth[i2];
            if (state->quad) {
                gte_AverageZ4(otz, z1, z2, state->depth[i3], &otz);
            } else {
                gte_AverageZ3(otz, z1, z2, &otz);
            }
            state->ot = state->otBase + otz;
            if (state->gouraud) {
                FIGHTSTG_addMeshPolyGT(state);
            } else {
                if (!state->lit) {
                    state->colors[0] = state->color[0];
                }
                FIGHTSTG_addMeshPolyFT(state);
            }
        } while (0);
        n = state->quad + 3;
        state->cmd += n;
        if (state->lit) {
            state->cmd += n;
        }
        if (state->textured) {
            state->cmd += n * 2;
        }
    } while (*state->cmd == 0);
}

/* A layer callback that draws a Mesh: walks its command bytes (see
   MeshDrawState), setting its texture (setMeshTexture), and adds each
   polygon that faces the camera to the layer's ordering table
   (drawMeshPolygons). The match depends on the mesh coming in as a void *,
   on the ?: for q and on each polygon's checks and drawing being a
   do-while (0) with breaks (both in drawMeshPolygons). */
void FIGHTSTG_drawMesh(void *arg, Layer *layer) {
    Mesh *mesh = arg;
    MATRIX m;
    MeshDrawState state;
    u32 op;
    s32 hi;
    s32 lo;

    gte_CompMatrix(&GsWSMATRIX, &mesh->matrix, &m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    if (FIGHTSTG_isMeshOnScreen(mesh, layer) == 0) {
        return;
    }
    FIGHTSTG_projectMesh(mesh, layer);
    FIGHTSTG_lightMesh(mesh);
    state.cmd = mesh->commands;
    state.texPos = mesh->texPos;
    state.screen = mesh->screen;
    state.depth = mesh->depth;
    state.normalColors = mesh->colors;
    state.otBase = layer->getOt(layer);
    state.prim.ptr = GFX.funcs.getPrim();
    while (*state.cmd != 0xFF) {
        op = *state.cmd;
        hi = op >> 4;
        lo = op & 0xF;
        if (hi != 0) {
            switch (hi) {
            case 8:
                state.quad = lo;
                break;
            case 9:
                state.textured = lo;
                break;
            case 10:
                state.unk8 = lo;
                break;
            case 11:
                state.unk4 = lo;
                break;
            case 12:
                state.lit = lo;
                break;
            case 13:
                state.gouraud = lo;
                break;
            case 14:
                state.abr = lo;
                break;
            }
            state.cmd++;
        } else {
            switch (lo) {
            case 1:
                setMeshTexture(&state);
                break;
            case 2:
            case 3:
            case 4:
            case 5:
                if (mesh->colorMode != 0) {
                    state.color[lo - 2].r = mesh->color.r;
                    state.color[lo - 2].g = mesh->color.g;
                    state.color[lo - 2].b = mesh->color.b;
                } else {
                    state.color[lo - 2].r = state.cmd[1];
                    state.color[lo - 2].g = state.cmd[2];
                    state.color[lo - 2].b = state.cmd[3];
                }
                state.cmd += 4;
                break;
            case 0:
                drawMeshPolygons(&state);
                break;
            }
        }
    }
    GFX.funcs.setPrim(state.prim.ptr);
}

/* A primitive tag's word with its address replaced: the top byte of tag and
   the low 24 bits of addr. The match depends on word being set twice: as one
   expression, sched1 moves the tag's mask after the address's, which swaps
   their registers. */
static inline u_long linkTag(u_long tag, u_long addr) {
    u_long word = tag & 0xFF000000;
    word |= addr;
    return word;
}

/* Draws a Mesh as wireframe: each polygon of its command runs as a green
   LINE_F4 through its screen points, with a LINE_F2 to close a quad. Each
   addPrim is written out with linkTag. */
void FIGHTSTG_drawMeshWireframe(Mesh *mesh, Layer *layer) {
    MATRIX m;
    MeshDrawState state;
    u32 op;
    s32 hi;
    s32 lo;
    s32 i0, i1, i2, i3;
    s32 n;
    u_long tag;

    gte_CompMatrix(&GsWSMATRIX, &mesh->matrix, &m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    if (FIGHTSTG_isMeshOnScreen(mesh, layer) == 0) {
        return;
    }
    FIGHTSTG_projectMesh(mesh, layer);
    state.cmd = mesh->commands;
    state.texPos = mesh->texPos;
    state.screen = mesh->screen;
    state.depth = mesh->depth;
    state.normalColors = mesh->colors;
    state.otBase = layer->getOt(layer);
    state.ot = state.otBase;
    state.prim.ptr = GFX.funcs.getPrim();
    while (*state.cmd != 0xFF) {
        op = *state.cmd;
        hi = op >> 4;
        lo = op & 0xF;
        if (hi != 0) {
            switch (hi) {
            case 8:
                state.quad = lo;
                break;
            case 9:
                state.textured = lo;
                break;
            case 12:
                state.lit = lo;
                break;
            }
            state.cmd++;
        } else {
            switch (lo) {
            case 1:
                state.cmd += 7;
                break;
            case 2:
            case 3:
            case 4:
            case 5:
                state.cmd += 4;
                break;
            case 0:
                do {
                    state.cmd++;
                    i0 = state.cmd[0];
                    i1 = state.cmd[1];
                    i2 = state.cmd[2];
                    i3 = 0;
                    if (state.quad) {
                        i3 = state.cmd[3];
                    }
                    state.sxy[0] = state.screen[i0];
                    state.sxy[1] = state.screen[i1];
                    state.sxy[2] = state.screen[i2];
                    if (state.quad) {
                        state.sxy[3] = state.screen[i3];
                    }
                    setLineF4(state.prim.lineF4);
                    setRGB0(state.prim.lineF4, 0, 0xFF, 0);
                    *(s32 *)&state.prim.lineF4->x0 = state.sxy[0];
                    *(s32 *)&state.prim.lineF4->x1 = state.sxy[1];
                    if (!state.quad) {
                        *(s32 *)&state.prim.lineF4->x2 = state.sxy[2];
                        *(s32 *)&state.prim.lineF4->x3 = state.sxy[0];
                    } else {
                        *(s32 *)&state.prim.lineF4->x2 = state.sxy[3];
                        *(s32 *)&state.prim.lineF4->x3 = state.sxy[2];
                    }
                    tag = *state.prim.tag;
                    *state.prim.tag = linkTag(tag, getaddr(state.ot));
                    tag = *state.ot;
                    *state.ot = linkTag(tag, (u_long)state.prim.ptr & 0xFFFFFF);
                    state.prim.lineF4++;
                    if (state.quad) {
                        setLineF2(state.prim.lineF2);
                        setRGB0(state.prim.lineF2, 0, 0xFF, 0);
                        *(s32 *)&state.prim.lineF2->x0 = state.sxy[2];
                        *(s32 *)&state.prim.lineF2->x1 = state.sxy[0];
                        tag = *state.prim.tag;
                        *state.prim.tag = linkTag(tag, getaddr(state.ot));
                        tag = *state.ot;
                        *state.ot = linkTag(tag, (u_long)state.prim.ptr & 0xFFFFFF);
                        state.prim.lineF2++;
                    }
                    n = state.quad + 3;
                    state.cmd += n;
                    if (state.lit) {
                        state.cmd += n;
                    }
                    if (state.textured) {
                        state.cmd += n * 2;
                    }
                } while (*state.cmd == 0);
                break;
            }
        }
    }
    GFX.funcs.setPrim(state.prim.ptr);
}

/* Mesh.draw: has the layer draw the Mesh (FIGHTSTG_drawMesh) with MATRIX */
void FIGHTSTG_queueMeshDraw(Mesh *mesh, s32 layerId, MATRIX *matrix) {
    Layer *layer = GFX.funcs.getLayer(layerId);

    layer->addCallback(layer, FIGHTSTG_drawMesh, mesh);
    mesh->matrix = *matrix;
}

/* Mesh.drawWireframe: has the layer draw the Mesh as wireframe
   (FIGHTSTG_drawMeshWireframe) with MATRIX */
void FIGHTSTG_queueMeshWireframe(Mesh *mesh, s32 layerId, MATRIX *matrix) {
    Layer *layer = GFX.funcs.getLayer(layerId);

    layer->addCallback(layer, FIGHTSTG_drawMeshWireframe, mesh);
    mesh->matrix = *matrix;
}

/* The Mesh's task: frees its screen positions, depths and colors when killed */
void FIGHTSTG_updateMesh(Mesh *mesh) {
    switch (mesh->state) {
    case TASK_INIT:
    case TASK_RUN:
    case TASK_DONE:
        break;
    case TASK_KILL:
        if (mesh->screen != NULL) {
            HEAP.free(mesh->screen);
        }
        if (mesh->depth != NULL) {
            HEAP.free(mesh->depth);
        }
        if (mesh->colors != NULL) {
            HEAP.free(mesh->colors);
        }
        break;
    }
}

/* Creates a Mesh from an archive: its vertices (entry 0), normals (1),
   commands (2) and bounds (5) */
Mesh *FIGHTSTG_createMesh(void *archive, Vec2 texPos) {
    Mesh *mesh = createTask(FIGHTSTG_updateMesh, sizeof(Mesh), 0);

    mesh->archive = archive;
    mesh->vertices = FILE_CACHE.getArchiveEntry(0, archive);
    mesh->normals = FILE_CACHE.getArchiveEntry(1, archive);
    mesh->commands = FILE_CACHE.getArchiveEntry(2, archive);
    mesh->bounds = FILE_CACHE.getArchiveEntry(5, archive);
    mesh->texPos = texPos;
    mesh->draw = FIGHTSTG_queueMeshDraw;
    mesh->drawWireframe = FIGHTSTG_queueMeshWireframe;
    return mesh;
}

/* the variables that end the file: zeros after camera_task.c's data, which
   the objects after this one read; the three unused words are read and
   written by nothing in either version */
s32 FIGHTSTG_unused0 = 0;
s32 FIGHTSTG_partnerIdleMotion = 0; /* the partner's idle motion while stage 0x1D is up */
s32 FIGHTSTG_unused1 = 0;
CameraView FIGHTSTG_fighterView = { 0 }; /* the view FIGHTSTG_getFighterView makes */
s32 FIGHTSTG_unused2 = 0;
RECT FIGHTSTG_fighterCameraRect = { 0 }; /* FIGHTSTG_updatePartnerView's layer */
DR_MOVE FIGHTSTG_cursorBarMoves[4] = { { 0 } }; /* FIGHTSTG_drawCursorBar draws its bar with them */
u_long FIGHTSTG_cursorBarOt[2] = { 0 }; /* and their ordering table */
BattleEvent FIGHTSTG_newEvent = { 0 }; /* what FIGHTSTG_pushEvent is given */
