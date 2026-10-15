#include "engine/game.h"

/* Draws a talk box's blinking "next" arrow where its type puts it */
void drawTalkBoxArrow(TalkBoxFrame *task) {
    SpriteDrawer obj;
    SVECTOR unused; /* unused, but it is in the original stack frame */
    s32 x;
    DVECTOR *arrow;
    DVECTOR *pos;

    pos = &TALK_BOX_LAYOUTS[task->parent->type].arrow;
    if ((u32)(task->parent->type - 2) < 2) {
        x = task->parent->w - 14;
    } else {
        x = pos->vx;
    }
    if (task->showArrow) {
        initSpriteDrawer(&obj);
        obj.setLayerId(task->parent->layerId, 0);
        obj.setTexture(0x140, 0);
        /* the match depends on this copy of pos: it puts pos in $s2 and x in $s3 */
        arrow = pos;
        if (GFX.funcs.getTime() - task->arrowTime >= 6) {
            task->arrowTime = GFX.funcs.getTime();
            if (++task->arrowFrame >= 4) {
                task->arrowFrame = 0;
            }
        }
        obj.setClutRow(task->arrowFrame);
        obj.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 7, task->parent->x + x, task->parent->y + arrow->vy);
    }
}

/* Draws a talk box's corner sprites and its semi-transparent panel */
void drawTalkBoxFrame(TalkBoxFrame *task) {
    SpriteDrawer obj;
    SVECTOR v[4];
    TalkBox *parent = task->parent;
    u32 type = parent->type;
    s32 widthOffset;
    s32 i;
    s32 x;
    FileCache *cache;
    u_long *ot;
    Layer *layer;
    POLY_FT4 *p;
    DVECTOR *pos;

    widthOffset = 0;
    if (type < 2) {
        widthOffset = parent->w;
    }
    pos = &TALK_BOX_LAYOUTS[type].spritePos;
    initSpriteDrawer(&obj);
    obj.setLayerId(task->parent->layerId, 0);
    obj.setTexture(0x140, 0);
    cache = &FILE_CACHE;
    for (i = 0; i < 4; pos++, i++) {
        switch (i) {
        case 0:
            obj.draw(cache->getEntry(FILE_MENU_SPRITES << 16), TALK_BOX_LAYOUTS[type].sprite, task->parent->x + pos->vx,
                           task->parent->y + pos->vy);
            break;
        case 1:
            obj.draw(cache->getEntry(FILE_MENU_SPRITES << 16), 0, task->parent->x + pos->vx - widthOffset, task->parent->y + pos->vy);
            break;
        case 3:
            if (type < 2) {
                obj.draw(cache->getEntry(FILE_MENU_SPRITES << 16), 2, task->parent->x + pos->vx, task->parent->y + pos->vy);
            } else {
                x = task->parent->w - 14;
                obj.draw(cache->getEntry(FILE_MENU_SPRITES << 16), 2, task->parent->x + x, task->parent->y + pos->vy);
            }
            break;
        }
    }
    layer = GFX.funcs.getLayer(task->parent->layerId);
    ot = layer->getOtEntry(layer, 0);
    /* By name: the code adds the panel's offset (12) to the table's address,
       where an array's &parts[2] adds 8 to the first position's */
    pos = &TALK_BOX_LAYOUTS[type].panel;
    v[0].vx = v[2].vx = task->parent->x + pos->vx - widthOffset;
    v[1].vx = v[3].vx = v[0].vx + task->parent->w;
    v[0].vy = v[1].vy = task->parent->y + pos->vy;
    v[2].vy = v[3].vy = v[0].vy + task->parent->h;
    v[0].vz = v[1].vz = v[2].vz = v[3].vz = 0;
    p = GFX.funcs.getPrim();
    setPolyFT4(p);
    setRGB0(p, 0x80, 0x80, 0x80);
    p->tpage = 0x45;
#if VERSION_US || VERSION_JP
    p->clut = 0x2E57;
#elif VERSION_EU
    p->clut = 0x2C57;
#endif
    setSemiTrans(p, 1);
    p->x0 = v[0].vx;
    p->y0 = v[0].vy;
    p->x1 = v[1].vx;
    p->y1 = v[1].vy;
    p->x2 = v[2].vx;
    p->y2 = v[2].vy;
    p->x3 = v[3].vx;
    p->y3 = v[3].vy;
    setUV4(p, 0xBC, 0, 0xC7, 0, 0xBC, 0x3E, 0xC7, 0x3E);
    addPrim(ot, p);
    GFX.funcs.setPrim(p + 1);
}

/* The talk box frame task: idle until the box opens, then draws the frame and the arrow */
void updateTalkBoxFrame(TalkBoxFrame *task) {
    switch (task->state) {
    case 0:
    default:
        task->setState(task, TASK_DONE);
        break;
    case 1:
        drawTalkBoxArrow(task);
        drawTalkBoxFrame(task);
        break;
    case 2:
    case 3:
        break;
    }
}

/* The frame of a talk box */
TalkBoxFrame *createTalkBoxFrame(TalkBox *parent) {
    TalkBoxFrame *task = createTask(updateTalkBoxFrame, sizeof(TalkBoxFrame), 0);

    task->parent = parent;
    return task;
}

typedef struct Order4 {
    s32 next[4];
} Order4;

/* The corner of the box each corner's side of the outline goes to */
const Order4 OUTLINE_ORDER = {{1, 3, 0, 2}};

/* Zooms the outline one frame in or out and draws it as four lines; ends when done */
void drawZoomBox(ZoomBox *task) {
    Layer *layer = GFX.funcs.getLayer(task->layerId);
    u_long *ot = layer->getOtEntry(layer, 0);
    SVECTOR out[4];
    SVECTOR in[4];
    LINE_F2 *line;
    Order4 order;
    s32 i;

    if (task->closing == 0) {
        task->zoom += task->speed;
        if (task->zoom > ONE) {
            task->zoom = ONE;
            task->opened = 1;
            task->setState(task, TASK_KILL);
        }
    } else {
        task->zoom -= task->speed;
        if (task->zoom < 0) {
            task->zoom = 0;
            task->setState(task, TASK_KILL);
        }
    }
    task->scale.vz = 0;
    task->scale.vx = task->scale.vy = task->zoom;
    RotMatrixYXZ_gte(&task->rot, &task->matrix);
    ScaleMatrix(&task->matrix, &task->scale);
    in[0].vx = in[2].vx = task->left - task->x;
    in[1].vx = in[3].vx = in[0].vx + task->w;
    in[0].vy = in[1].vy = task->top - task->y;
    in[2].vy = in[3].vy = in[0].vy + task->h;
    in[0].vz = in[1].vz = in[2].vz = in[3].vz = 0;
    for (i = 0; i < 4; i++) {
        ApplyMatrixSV(&task->matrix, &in[i], &out[i]);
        out[i].vx += task->x;
        out[i].vy += task->y;
    }
    line = GFX.funcs.getPrim();
    for (i = 0; i < 4; i++) {
        order = OUTLINE_ORDER;
        setLineF2(line);
        setRGB0(line, 0, 0, 0xFF);
        line->x0 = out[i].vx;
        line->y0 = out[i].vy;
        line->x1 = out[order.next[i]].vx;
        line->y1 = out[order.next[i]].vy;
        addPrim(ot, line);
        line++;
    }
    GFX.funcs.setPrim(line);
}

/* The zoom box task: starts zoomed out (opening) or in (closing), then draws */
void updateZoomBox(ZoomBox *task) {
    switch (task->state) {
    case 0:
    default:
        task->unused68 = 0;
        if (task->closing == 0) {
            task->zoom = 0;
        } else {
            task->zoom = ONE;
        }
        task->trans.vz = 0;
        task->trans.vx = task->x;
        task->trans.vy = task->y;
        task->nextState(task);
        break;
    case 1:
        drawZoomBox(task);
        break;
    case 2:
    case 3:
        break;
    }
}

/* A zoom box around a talk box of type `type` at (x, y) */
ZoomBox *createZoomBox(s32 layerId, s16 x, s16 y, s32 w, s32 h, s32 type) {
    s16 widthOffset = w;
    ZoomBox *task = createTask(updateZoomBox, sizeof(ZoomBox), 0);

    task->layerId = layerId;
    task->x = x;
    task->y = y;
    task->w = w + 0x20;
    task->h = h;
    if (type == 2 || type == 3) {
        widthOffset = 0;
    }
    task->offsetX = TALK_BOX_LAYOUTS[type].sprite0Pos.vx - widthOffset;
    task->offsetY = TALK_BOX_LAYOUTS[type].sprite0Pos.vy;
    task->left = x + task->offsetX;
    task->top = y + task->offsetY;
    return task;
}

/* Talk box method: moves the box with its outlines and windows */
void talkBoxSetPos(TalkBox *task, s32 x, s32 y) {
    TalkBoxChildren *children = task->children;
    s32 i;
    u32 type;
    s32 wx;
    s32 wy;
    ZoomBox *item;

    task->x = x;
    task->y = y;
    for (i = 0; i < 3; i++) {
        item = children->zoomBoxes[i];
        if (item != NULL && item->state == 1) {
            item->left = item->offsetX + x;
            item->top = item->offsetY + y;
        }
    }
    type = task->type;
    if (children->windows[0] != NULL) {
        wx = task->x + TALK_BOX_LAYOUTS[type].nameX;
        wy = task->y + TALK_BOX_LAYOUTS[type].nameY;
        if (type < 2) {
            wx -= task->w;
        }
        children->windows[0]->setPos(children->windows[0], wx, wy);
    }
    if (children->windows[1] != NULL) {
        wx = task->x + TALK_BOX_LAYOUTS[type].textX;
        wy = task->y + TALK_BOX_LAYOUTS[type].textY;
        if (type < 2) {
            wx -= task->w;
        }
        children->windows[1]->setPos(children->windows[1], wx, wy);
    }
}

typedef struct Delays3 {
    s32 frames[3];
} Delays3;

/* The frames before each of the three zoom boxes of an opening or closing */
const Delays3 ZOOM_BOX_DELAYS = {{0, 2, 4}};

/* The talk box task: opens with its outlines, runs the text, then closes and ends */
void updateTalkBox(TalkBox *task, TalkBoxChildren *children) {
    Delays3 delays;

    switch (task->state) {
    case 0:
    default:
        task->setState(task, TASK_DONE);
        break;
    case 1:
        if (children->windows[1]->isFinished(children->windows[1]) != 0) {
            task->setState(task, TASK_DONE);
            task->setSubstate(task, 1);
            children->frame->setState(children->frame, TASK_DONE);
            children->windows[0]->setVisible(children->windows[0], 0);
            children->windows[1]->setVisible(children->windows[1], 0);
        } else if (PAD_PRESSED(PAD_CROSS)) {
            children->windows[1]->showPage(children->windows[1]);
        }
        if (children->windows[1]->isWaitingForButton(children->windows[1]) != 0) {
            children->frame->showArrow = 1;
        } else {
            children->frame->showArrow = 0;
        }
        break;
    case 2:
        delays = ZOOM_BOX_DELAYS;
        switch (task->step) {
        case 0:
        default:
            if (task->substate == 0) {
                SOUND.playSound(SOUND_MENU_OPEN);
            } else {
                SOUND.playSound(SOUND_MENU_CLOSE);
            }
            /* fallthrough */
        case 1:
        case 2:
            if (task->timer++ >= delays.frames[task->step]) {
                children->zoomBoxes[task->step] = createZoomBox(task->layerId, task->x, task->y, task->w, task->h, task->type);
                children->zoomBoxes[task->step]->closing = task->substate;
                if (task->substate == 0) {
                    children->zoomBoxes[task->step]->speed = 0x199;
                } else {
                    children->zoomBoxes[task->step]->speed = 0x333;
                }
                task->nextStep(task);
                task->timer = 0;
            }
            break;
        case 3:
            if (task->substate == 0) {
                if (children->zoomBoxes[2]->opened != 0) {
                    task->setState(task, TASK_RUN);
                    children->frame->setState(children->frame, TASK_RUN);
                    children->windows[0]->setVisible(children->windows[0], 1);
                    children->windows[1]->setVisible(children->windows[1], 1);
                }
            } else if (task->substate == 1) {
                if (children->zoomBoxes[2] == NULL) {
                    task->setState(task, TASK_KILL);
                }
            }
            break;
        }
        break;
    case 3:
        break;
    }
}

/*
 * A talk box with string `index` of a table (an optional speaker's name between 02 07 codes),
 * sized to the text
 */
TalkBox *createTalkBox(s32 id, s16 x, s16 y, void *strings, s32 index, u32 type) {
    TextTools fn;
    char name[0x20];
    TalkBox *task;
    TalkBoxChildren *children;
    s32 end;
    u8 *text;
    s32 w;
    s32 i;

    task = createTask(updateTalkBox, sizeof(TalkBox), sizeof(TalkBoxChildren));
    task->layerId = id;
    task->x = x;
    task->y = y;
    children = task->children;
    task->setPos = talkBoxSetPos;
    task->type = type;
    initTextTools(&fn);
    i = 0;
    children->windows[0] = createTextWindow(task->layerId, 2, task->x + TALK_BOX_LAYOUTS[type].nameX, task->y + TALK_BOX_LAYOUTS[type].nameY);
    children->windows[0]->setLines(children->windows[0], 3);
    children->windows[1] = createTextWindow(task->layerId, 1, task->x + TALK_BOX_LAYOUTS[type].textX, task->y + TALK_BOX_LAYOUTS[type].textY);
    children->windows[1]->setLines(children->windows[1], 3);
    task->strings = strings;
    text = fn.getString(strings, index);
    if (text[0] == 2 && text[1] == 7) {
        end = 2;
        while (text[end] != 2 && text[end + 1] != 7) {
            end++;
        }
        HEAP.zero(name, sizeof(name));
        if (text[i + 2] == 2 && text[i + 3] == 9) {
            strcpy(name, GAME.name);
        } else {
            strncpy(name, &text[i + 2], -2 - i + end);
        }
        children->windows[0]->setString(children->windows[0], name, -1);
        children->windows[1]->setString(children->windows[1], &text[end + 2], -1);
    } else {
        children->windows[1]->setString(children->windows[1], text, -1);
    }
    children->windows[1]->setTypeDelay(children->windows[1], 6);
    children->windows[1]->insertPlayerName(children->windows[1]);
    w = fn.measure(children->windows[1]->text, children->windows[1]->style, 0);
    if (w < 0x5F) {
        w = 0x5F;
    } else if (w >= 0x8C) {
        w = 0x8B;
    }
    task->w = w;
    task->h = 0x3E;
    if (type < 2) {
        children->windows[0]->setPos(children->windows[0], children->windows[0]->x - task->w, children->windows[0]->y);
        children->windows[1]->setPos(children->windows[1], children->windows[1]->x - task->w, children->windows[1]->y);
    }
    for (i = 0; i < 2; i++) {
        children->windows[i]->setPalette(children->windows[i], PALETTE_DARK_BLUE);
        children->windows[i]->setVisible(children->windows[i], 0);
    }
    children->frame = createTalkBoxFrame(task);
    return task;
}

TalkBoxLayout TALK_BOX_LAYOUTS[4] = {
    { 3, { 0, -16 }, { 0, -74 }, { 16, -74 }, { 16, -74 }, 6, -71, 6, -58, { 16, -38 } },
    { 5, { 0, 0 }, { 0, 12 }, { 16, 12 }, { 16, 12 }, 6, 15, 6, 28, { 16, 48 } },
    { 4, { -16, -16 }, { -26, -74 }, { -10, -74 }, { 125, -74 }, -20, -71, -20, -58, { 125, -38 } },
    { 6, { -20, 0 }, { -26, 12 }, { -10, 12 }, { 125, 12 }, -20, 15, -20, 28, { 125, 48 } },
};
