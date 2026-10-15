#include "menus/country_select.h"

RECT CNTY_SEL_screenRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
RECT CNTY_SEL_vramRect = {0, 0, 1024, 512};
/* A little larger than the screen */
RECT CNTY_SEL_fadeRect = {0, -15, 320, 260};

/* Sprite frames of the chosen option once Start is pressed */
AnimFrame CNTY_SEL_cursorFlash[] = {
    {8, 4}, {9, 4}, {10, 4}, {11, 30}, {0xFF, 999},
};

/* Opening (0) and closing (1) tweens of each panel's scale */
PanelTween CNTY_SEL_topPanelTweens[] = {
    {10, 0x1000, 0},
    {5, 0, 0x1000},
};
PanelTween CNTY_SEL_rightPanelTweens[] = {
    {10, 0x1000, 0},
    {5, 0, 0x1000},
};
LeftPanelTween CNTY_SEL_leftPanelTweens[] = {
    {10, 0x1000, 0},
    {5, 0, 0x1000},
};

#if VERSION_EU
/* The language of each option */
u8 CNTY_SEL_languages[] = {2, 3, 5, 4, 6, 1, 0};
#endif

/* The mode's root task: once the sound bank is loaded, sets up the display and the layer,
   uploads the images, creates the screen's controller and starts the music; stops the
   music when killed */
void CNTY_SEL_tickScreen(Task *task, CountryMenuTask **menu) {
    TimLoader loader;
    Layer *layer;

    switch (task->state) {
    case TASK_INIT:
    default:
        /* Wait for the sound bank requested by CNTY_SEL_start */
        if (SOUND.isLoading() == 0) {
            /* Free every draw layer, get two 0xA000-byte primitive buffers
               and a 320x240 display */
            GFX.funcs.reset();
            GFX.funcs.allocPrimBuffers(0xA000);
            GFX.funcs.setDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
            /* Upload the images to VRAM from (640, 0), their CLUTs from (0, 496) */
            initTimLoader(&loader);
            loader.setImagePos(0x280, 0);
            loader.setClutPos(0, 0x1F0);
            loader.loadArchive(FILE_CACHE.getEntry(CNTY_SEL_IMAGES));
            /* A full-screen layer cleared to dark gray */
            layer = GFX.funcs.createLayer(&CNTY_SEL_screenRect, 3, CNTY_SEL_LAYER);
            layer->setBgColor(layer, 0x1F, 0x1F, 0x1F);
            *menu = CNTY_SEL_startMenuTask();
            task->nextState(task);
            SOUND.playSound(CNTY_SEL_MUSIC);
        }
        break;
    case TASK_RUN:
    case TASK_DONE:
        break;
    case TASK_KILL:
        SOUND.stopSound(CNTY_SEL_MUSIC);
        break;
    }
}

/* The mode's entry point (MODE_ENTRY_POINTS): clears VRAM, starts the root task and
   loads the screen's sound bank */
Task *CNTY_SEL_start(void) {
    Task *task;

    ClearImage2(&CNTY_SEL_vramRect, 0, 0, 0);
    task = createTask(CNTY_SEL_tickScreen, sizeof(Task), sizeof(CountryMenuTask *));
    SOUND.loadBank(CNTY_SEL_SOUND_BANK);
    return task;
}

/* Draws the background, moved diagonally by half its scroll */
void CNTY_SEL_drawBackground(CountryBackgroundTask *task) {
    SpriteDrawer sprite;
    s32 offset;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(CNTY_SEL_LAYER, 1);
    sprite.setTexture(0x280, 0);
    /* The background repeats every 96 pixels */
    offset = (s16)((s16)(task->scroll / 2) % 96);
    sprite.draw(FILE_CACHE.getEntry(CNTY_SEL_SPRITES), SPRITE_BACKGROUND, offset, offset);
}

/* How dark the fade-out is on its frame `time`: 0 to 255 over 30 frames */
s32 CNTY_SEL_getFadeLevel(s32 time) {
    if (time >= 30) {
        return 255;
    }
    return time * 255 / 30;
}

/* Darkens the whole screen by level (0 to 255, 255 is black) */
void CNTY_SEL_drawFade(s32 level) {
    Layer *layer = GFX.funcs.getLayer(CNTY_SEL_LAYER);
    u_long *ot = layer->getOtEntry(layer, 0);
    POLY_F4 *poly = GFX.funcs.getPrim();
    DR_TPAGE *mode;

    setlen(poly, 5);
    poly->r0 = poly->g0 = poly->b0 = level;
    setcode(poly, 0x2A); /* semi-transparent POLY_F4 */
    poly->x0 = CNTY_SEL_fadeRect.x;
    poly->x1 = CNTY_SEL_fadeRect.x + CNTY_SEL_fadeRect.w;
    poly->x2 = CNTY_SEL_fadeRect.x;
    poly->x3 = CNTY_SEL_fadeRect.x + CNTY_SEL_fadeRect.w;
    poly->y0 = CNTY_SEL_fadeRect.y;
    poly->y1 = CNTY_SEL_fadeRect.y;
    poly->y2 = CNTY_SEL_fadeRect.y + CNTY_SEL_fadeRect.h;
    poly->y3 = CNTY_SEL_fadeRect.y + CNTY_SEL_fadeRect.h;
    /* the texture page packet goes right after the polygon */
    mode = (DR_TPAGE *)(poly + 1);
    addPrim(ot, poly);
    /* Blending mode 2: subtract the polygon's color from the screen */
    setDrawTPage(mode, 0, 1, getTPage(0, 2, 320, 0));
    addPrim(ot, mode);
    GFX.funcs.setPrim(mode + 1);
}

/* The background's task: scrolls and draws it; when done, keeps it scrolling while the
   screen fades to black over 30 frames, then ends */
void CNTY_SEL_tickBackground(CountryBackgroundTask *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        task->scroll = 0;
        break;
    case TASK_RUN:
        task->scroll = (s16)(task->scroll + 1) % 192;
        CNTY_SEL_drawBackground(task);
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            task->fade = 0;
            task->substate = 1;
        }
        task->scroll = (s16)(task->scroll + 1) % 192;
        CNTY_SEL_drawBackground(task);
        CNTY_SEL_drawFade(CNTY_SEL_getFadeLevel(task->fade++));
        if (task->fade >= 30) {
            task->setState(task, TASK_KILL);
        }
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates the scrolling background (task) */
CountryBackgroundTask *CNTY_SEL_startBackgroundTask(void) {
    return createTask(CNTY_SEL_tickBackground, sizeof(CountryBackgroundTask), 0);
}

/* Advances anim by the frames elapsed and returns the frame to show (0xFF at the end) */
s16 CNTY_SEL_stepAnimation(AnimState *anim, AnimFrame *frames, s32 depth) {
    AnimFrame *frame = &frames[anim->index];
    s32 elapsed = GFX.funcs.getFrameTime();

    if (elapsed > 4) {
        elapsed = 4;
    }
    if (depth == 0) {
        anim->timer -= elapsed;
    }
    if (anim->timer <= 0) {
        frame++;
        anim->index++;
        anim->timer += frame->duration;
        if (frame->frame == 0xFF) {
            return 0xFF;
        }
        /* Skip the frames that were shorter than the time elapsed */
        CNTY_SEL_stepAnimation(anim, frames, depth + 1);
    }
    return frame->frame;
}

/* Draws the highlighted option, in the colors of the CLUT row of its current frame */
void CNTY_SEL_drawCursor(CursorTask *task) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(CNTY_SEL_LAYER, 1);
    sprite.setTexture(0x280, 0);
    sprite.setClutRow(task->frame);
    sprite.draw(FILE_CACHE.getEntry(CNTY_SEL_SPRITES), SPRITE_OPTIONS + task->selection, 0, 0);
}

/* Moves the highlight to option `selection` (CursorTask.setSelection) */
void CNTY_SEL_setCursorSelection(CursorTask *task, s16 selection) {
    task->selection = selection;
}

/* The highlighted option's task: blinks it through 8 CLUT rows, 4 frames each; when
   done (Start pressed), flashes it with CNTY_SEL_cursorFlash, then ends */
void CNTY_SEL_tickCursor(CursorTask *task) {
    s16 frame;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        break;
    case TASK_RUN:
        task->blink++;
        task->blink %= 32;
        task->frame = task->blink / 4;
        CNTY_SEL_drawCursor(task);
        break;
    case TASK_DONE:
        if (task->substate == 0) {
            task->anim.index = 0;
            task->anim.timer = CNTY_SEL_cursorFlash[0].duration;
            task->substate++;
        }
        frame = CNTY_SEL_stepAnimation(&task->anim, CNTY_SEL_cursorFlash, 0);
        task->frame = frame;
        if (frame != 0xFF) {
            CNTY_SEL_drawCursor(task);
        } else {
            task->setState(task, TASK_KILL);
        }
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates the highlighted option (task), once the panels are open */
CursorTask *CNTY_SEL_startCursorTask(void) {
    CursorTask *task = createTask(CNTY_SEL_tickCursor, sizeof(CursorTask), 0);

    task->setSelection = CNTY_SEL_setCursorSelection;
    return task;
}

/* The top panel's height scale on the current frame of its opening (phase 0) or
   closing (1) */
s32 CNTY_SEL_getTopPanelScale(PanelTask *task, s32 phase) {
    s32 time = task->time;
    s32 duration = CNTY_SEL_topPanelTweens[phase].duration;

    if (time >= duration) {
        return CNTY_SEL_topPanelTweens[phase].to;
    }
    return CNTY_SEL_topPanelTweens[phase].from +
           (CNTY_SEL_topPanelTweens[phase].to - CNTY_SEL_topPanelTweens[phase].from) * time / duration;
}

/* Draws the top panel, its height scaled from its top edge */
void CNTY_SEL_drawTopPanel(PanelTask *task) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(CNTY_SEL_LAYER, 1);
    sprite.setTexture(0x280, 0);
    sprite.setPivot(148, 0);
    sprite.setScale(task->scaleX, task->scaleY, ONE);
    sprite.draw(FILE_CACHE.getEntry(CNTY_SEL_SPRITES), SPRITE_TOP_PANEL, 148, 0);
}

/* The top panel's task: draws it; each TASK_DONE from the controller opens it, the
   next one closes it */
void CNTY_SEL_tickTopPanel(PanelTask *task) {
    s16 phase;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        task->scaleX = ONE;
        task->scaleY = 0;
        task->phase = 0;
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
            break;
        case 1:
            phase = task->phase;
            if (CNTY_SEL_topPanelTweens[phase].duration < task->time++) {
                task->phase ^= 1;
                task->setSubstate(task, 0);
                task->scaleY = CNTY_SEL_topPanelTweens[phase].to;
            } else {
                task->scaleY = CNTY_SEL_getTopPanelScale(task, phase);
            }
            break;
        }
        CNTY_SEL_drawTopPanel(task);
        break;
    case TASK_DONE:
        task->time = 0;
        task->setState(task, TASK_RUN);
        task->setSubstate(task, 1);
        CNTY_SEL_drawTopPanel(task);
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates the top panel (task), closed */
PanelTask *CNTY_SEL_startTopPanelTask(void) {
    return createTask(CNTY_SEL_tickTopPanel, sizeof(PanelTask), 0);
}

/* The right panel's width scale on the current frame of its opening (phase 0) or
   closing (1) */
s32 CNTY_SEL_getRightPanelScale(PanelTask *task, s32 phase) {
    s32 time = task->time;
    s32 duration = CNTY_SEL_rightPanelTweens[phase].duration;

    if (time >= duration) {
        return CNTY_SEL_rightPanelTweens[phase].to;
    }
    return CNTY_SEL_rightPanelTweens[phase].from +
           (CNTY_SEL_rightPanelTweens[phase].to - CNTY_SEL_rightPanelTweens[phase].from) * time / duration;
}

/* Draws the right panel, its width scaled from the screen's right edge */
void CNTY_SEL_drawRightPanel(PanelTask *task) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(CNTY_SEL_LAYER, 1);
    sprite.setTexture(0x280, 0);
    sprite.setPivot(320, 20);
    sprite.setScale(task->scaleX, task->scaleY, ONE);
    sprite.draw(FILE_CACHE.getEntry(CNTY_SEL_SPRITES), SPRITE_RIGHT_PANEL, 320, 20);
}

/* The right panel's task: draws it; each TASK_DONE from the controller opens it, the
   next one closes it */
void CNTY_SEL_tickRightPanel(PanelTask *task) {
    s16 phase;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        task->scaleY = ONE;
        task->scaleX = 0;
        task->phase = 0;
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
            break;
        case 1:
            phase = task->phase;
            if (CNTY_SEL_rightPanelTweens[phase].duration < task->time++) {
                task->phase ^= 1;
                task->setSubstate(task, 0);
                task->scaleX = CNTY_SEL_rightPanelTweens[phase].to;
            } else {
                task->scaleX = CNTY_SEL_getRightPanelScale(task, phase);
            }
            break;
        }
        CNTY_SEL_drawRightPanel(task);
        break;
    case TASK_DONE:
        task->time = 0;
        task->setState(task, TASK_RUN);
        task->setSubstate(task, 1);
        CNTY_SEL_drawRightPanel(task);
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates the right panel (task), closed */
PanelTask *CNTY_SEL_startRightPanelTask(void) {
    return createTask(CNTY_SEL_tickRightPanel, sizeof(PanelTask), 0);
}

/* The left panel's width scale on the current frame of its opening (phase 0) or
   closing (1) */
s32 CNTY_SEL_getLeftPanelScale(PanelTask *task, s32 phase) {
    s32 time = task->time;
    s32 duration = CNTY_SEL_leftPanelTweens[phase].duration;

    if (time >= duration) {
        return CNTY_SEL_leftPanelTweens[phase].to;
    }
    return CNTY_SEL_leftPanelTweens[phase].from +
           (CNTY_SEL_leftPanelTweens[phase].to - CNTY_SEL_leftPanelTweens[phase].from) * time / duration;
}

/* Draws the left panel, with its own CLUT, its width scaled from the screen's left
   edge */
void CNTY_SEL_drawLeftPanel(PanelTask *task) {
    SpriteDrawer sprite;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(CNTY_SEL_LAYER, 1);
    sprite.setTexture(0x280, 0);
    sprite.setAltClut(0, 0x1F0);
    sprite.setPivot(0, 158);
    sprite.setScale(task->scaleX, task->scaleY, ONE);
    sprite.draw(FILE_CACHE.getEntry(CNTY_SEL_SPRITES), SPRITE_LEFT_PANEL, 0, 158);
}

/* The left panel's task: draws it; each TASK_DONE from the controller opens it, the
   next one closes it */
void CNTY_SEL_tickLeftPanel(PanelTask *task) {
    s16 phase;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        task->scaleY = ONE;
        task->scaleX = 0;
        task->phase = 0;
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
            break;
        case 1:
            phase = task->phase;
            if (CNTY_SEL_leftPanelTweens[phase].duration < task->time++) {
                task->phase ^= 1;
                task->setSubstate(task, 0);
                task->scaleX = CNTY_SEL_leftPanelTweens[phase].to;
            } else {
                task->scaleX = CNTY_SEL_getLeftPanelScale(task, phase);
            }
            break;
        }
        CNTY_SEL_drawLeftPanel(task);
        break;
    case TASK_DONE:
        task->time = 0;
        task->setState(task, TASK_RUN);
        task->setSubstate(task, 1);
        CNTY_SEL_drawLeftPanel(task);
        break;
    case TASK_KILL:
        break;
    }
}

/* Creates the left panel (task), closed */
PanelTask *CNTY_SEL_startLeftPanelTask(void) {
    return createTask(CNTY_SEL_tickLeftPanel, sizeof(PanelTask), 0);
}

/* Moves the highlighted option with the pad */
static inline void moveSelection(CountryMenuTask *task) {
    /* Options 0-4 are a column moved through with Up and Down. Options 5
       and 6 are a second one reached with step 1, which nothing sets */
    switch (task->step) {
    case 0:
    default:
        if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
            if (task->selection > 0) {
                SOUND.playSound(SOUND_MENU_MOVE);
                task->selection--;
            }
        } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
            if (task->selection < 4) {
                SOUND.playSound(SOUND_MENU_MOVE);
                task->selection++;
            }
        } else if (PAD_PRESSED(PAD_LEFT)) {
            /* Left does nothing in this column */
        }
        break;
    case 1:
        if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
            if (task->selection != 6) {
                SOUND.playSound(SOUND_MENU_MOVE);
            }
            task->selection = 6;
        } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
            if (task->selection == 6) {
                SOUND.playSound(SOUND_MENU_MOVE);
                task->selection = 5;
            } else {
                task->selection = 1;
                SOUND.playSound(SOUND_MENU_MOVE);
                task->setStep(task, 0);
            }
        }
        break;
    }
}

/* The screen's controller: opens the panels one after the other and lets the player
   pick an option with Up and Down until Start, which in the European version sets
   LANGUAGE; then flashes it, closes the panels, fades out and moves on to the next mode */
void CNTY_SEL_tickMenu(CountryMenuTask *task, MenuChildren *children) {
    switch (task->state) {
    case TASK_INIT:
    default:
        children->background = CNTY_SEL_startBackgroundTask();
        children->topPanel = CNTY_SEL_startTopPanelTask();
        children->rightPanel = CNTY_SEL_startRightPanelTask();
        children->leftPanel = CNTY_SEL_startLeftPanelTask();
        task->selection = 0;
        task->timer = 0;
        task->nextState(task);
        break;
    case TASK_RUN:
        switch (task->substate) {
        case MENU_OPEN_RIGHT_PANEL:
        default:
            children->rightPanel->setState(children->rightPanel, TASK_DONE);
            task->timer = 0;
            task->nextSubstate(task);
            break;
        case MENU_WAIT_RIGHT_PANEL:
            if (task->timer++ >= 6) {
                task->nextSubstate(task);
            }
            break;
        case MENU_OPEN_LEFT_PANEL:
            task->timer = 0;
            children->leftPanel->setState(children->leftPanel, TASK_DONE);
            task->nextSubstate(task);
            break;
        case MENU_WAIT_LEFT_PANEL:
            if (task->timer++ >= 11) {
                task->nextSubstate(task);
            }
            break;
        case MENU_OPEN_TOP_PANEL:
            task->timer = 0;
            children->topPanel->setState(children->topPanel, TASK_DONE);
            task->nextSubstate(task);
            break;
        case MENU_WAIT_TOP_PANEL:
            if (task->timer++ >= 6) {
                task->nextSubstate(task);
            }
            break;
        case MENU_SHOW_CURSOR:
            children->cursor = CNTY_SEL_startCursorTask();
            task->nextSubstate(task);
            break;
        case MENU_SELECT:
            if (PAD_PRESSED(PAD_START)) {
                SOUND.playSound(SOUND_MENU_CONFIRM);
                children->cursor->setState(children->cursor, TASK_DONE);
                task->nextSubstate(task);
                task->timer = 0;
#if VERSION_EU
                LANGUAGE = CNTY_SEL_languages[task->selection];
#endif
            } else {
                moveSelection(task);
            }
            children->cursor->setSelection(children->cursor, task->selection);
            break;
        case MENU_WAIT_FLASH:
            if (++task->timer >= 43) {
                task->nextSubstate(task);
            }
            break;
        case MENU_CLOSE_PANELS:
            children->rightPanel->setState(children->rightPanel, TASK_DONE);
            children->leftPanel->setState(children->leftPanel, TASK_DONE);
            children->topPanel->setState(children->topPanel, TASK_DONE);
            task->timer = 0;
            task->nextSubstate(task);
            break;
        case MENU_WAIT_PANELS:
            if (++task->timer >= 6) {
                task->nextSubstate(task);
            }
            break;
        case MENU_FADE_OUT:
            children->background->setState(children->background, TASK_DONE);
            task->nextSubstate(task);
            break;
        case MENU_WAIT_FADE:
            if (++task->timer >= 31) {
                task->nextSubstate(task);
            }
            break;
        case MENU_EXIT:
#if VERSION_US || VERSION_JP
            GAME.funcs.requestMode(MODE_OPENING, 0);
#elif VERSION_EU
            if (LANGUAGE == 0) {
                GAME.funcs.requestMode(MODE_OPENING, 0);
            } else {
                GAME.funcs.requestMode(MODE_OPENING_2, 0);
            }
#endif
            task->setState(task, TASK_KILL);
            break;
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the screen's controller (task), which starts the background and panels */
CountryMenuTask *CNTY_SEL_startMenuTask(void) {
    return createTask(CNTY_SEL_tickMenu, sizeof(CountryMenuTask), sizeof(MenuChildren));
}
