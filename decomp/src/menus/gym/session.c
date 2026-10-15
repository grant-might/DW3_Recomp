/* A training session. It begins STGTRAIN.PRO's third object (see sprite.c),
   whose rodata starts at 0x800825E8 (USA) */

#include "menus/gym.h"

/* Creates the training session's text windows and cursor */
void STGTRAIN_createSessionWindows(TrainSession *session, TrainSessionWindows *win) {
    win->text[0] = createTextWindow(session->layerId, 1, 0xA2, 0x49);
    win->text[1] = createTextWindow(session->layerId, 1, 0xBC, 0x14);
    win->text[2] = createTextWindow(session->layerId, 1, 0xC0, 0x29);
    win->intensities[0] = createTextWindow(session->layerId, 1, 0x98, 0x66);
    win->intensities[1] = createTextWindow(session->layerId, 1, 0xC0, 0x66);
    win->intensities[2] = createTextWindow(session->layerId, 1, 0xE6, 0x66);
    win->notice = createTextWindow(session->layerId, 1, 0x94, 0x87);
    win->answers[0] = createTextWindow(session->layerId, 1, 0xA2, 0x64);
    win->answers[1] = createTextWindow(session->layerId, 1, 0xA2, 0x74);
    win->cursor = createCursor(session->layerId, session->depth - 1, 0, 0);
    win->cursor->setVisible(win->cursor, 0);
}

/*
 * Draws a training session: the training's icon, its panels and the cursor
 * over the three columns.
 */
void STGTRAIN_drawSession(TrainSession *session) {
    SpriteDrawer sprite;
    s32 i;

    initSpriteDrawer(&sprite);
    sprite.setLayerId(session->layerId, session->depth);
    sprite.setTexture(0x240, 0x100);
    if (session->panels[2].level != 0) {
        if (session->panels[2].level != ONE) {
            sprite.setScale(session->panels[2].level, session->panels[2].level, ONE);
            sprite.setPivot(0xA6, 0x26);
        }
        if (GFX.funcs.getTime() - session->iconTime >= 0x10) {
            session->iconTime = GFX.funcs.getTime();
            session->iconFrame++;
            if (session->iconFrame >= 4) {
                session->iconFrame = 0;
            }
        }
        /* the match depends on i holding the icon too */
        i = STGTRAIN_state.trainings[session->screen->training].icons[session->iconFrame];
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_FILE_SPRITES << 16), i, 0x94, 0x14);
    }
    if (session->panels[3].level != 0) {
        if (session->panels[3].level != ONE) {
            sprite.setScale(session->panels[3].level, session->panels[3].level, ONE);
            sprite.setPivot(0xCE, 0x2F);
        }
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x2A, 0xBC, 0x26);
    }
    if (session->panels[1].level != 0) {
        sprite.setScale(session->panels[1].level, ONE, ONE);
        if (session->panels[1].level != ONE) {
            sprite.setPivot(0x140, 0x26);
        }
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x25, 0x8F, 0xF);
    }
    if (session->panels[0].level != 0) {
        if (session->panels[0].level != ONE) {
            sprite.setScale(session->panels[0].level, ONE, ONE);
            sprite.setPivot(0x140, 0x4E);
        }
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x26, 0x85, 0x43);
    }
    if (session->cursorShown != 0) {
        if (GFX.funcs.getTime() - session->cursorTime >= 0xB) {
            session->cursorTime = GFX.funcs.getTime();
            session->cursorClut++;
            if (session->cursorClut >= 4) {
                session->cursorClut = 0;
            }
        }
        sprite.setClutRow(session->cursorClut);
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x29, session->intensity * 40 + 0x94, 0x63);
        sprite.setClutRow(0);
    }
    if (session->panels[6].level != 0) {
        if (session->panels[6].level != ONE) {
            sprite.setScale(session->panels[6].level, session->panels[6].level, ONE);
        }
        for (i = 0; i < 3; i++) {
            if (session->panels[6].level != ONE) {
                sprite.setPivot(i * 40 + 0xA6, 0x6C);
            }
            sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x2A, i * 40 + 0x94, 0x63);
        }
    }
    if (session->panels[5].level != 0) {
        sprite.setScale(session->panels[5].level, ONE, ONE);
        sprite.setPivot(0x140, 0x6C);
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x27, 0x82, 0x5E);
    }
    if (session->panels[7].level != 0) {
        sprite.setScale(session->panels[7].level, ONE, ONE);
        sprite.setPivot(0x140, 0x8D);
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x27, 0x82, 0x7F);
    }
    if (session->panels[4].level != 0) {
        sprite.setScale(session->panels[4].level, ONE, ONE);
        sprite.setPivot(0x140, 0x72);
        sprite.draw(FILE_CACHE.getEntry(STGTRAIN_SPRITES), 0x1D, 0x82, 0x5E);
    }
}

/* Once the title panel is open, writes the training's name in it and opens
   the next panel */
static inline void STGTRAIN_showTrainingName(TrainSession *session, TrainSessionWindows *win) {
    s32 name;

    if (STGTRAIN_state.updateFade(&session->panels[1])) {
        name = STGTRAIN_state.trainings[session->screen->training].name;
        win->text[1]->setString(win->text[1], FILE_CACHE.load(STGTRAIN_TEXT), name);
        STGTRAIN_state.startFade(&session->panels[2], 1);
        session->substate++;
    }
}

/* Once the intensities' panel is open, writes the three intensities and
   shows the cursor */
static inline void STGTRAIN_showIntensities(TrainSession *session, TrainSessionWindows *win) {
    s32 i;

    if (STGTRAIN_state.updateFade(&session->panels[6])) {
        for (i = 0; i < 3; i++) {
            win->intensities[i]->setString(win->intensities[i], FILE_CACHE.load(STGTRAIN_TEXT), i + 0xE);
        }
        session->cursorShown = 1;
        session->substate++;
    }
}

/* Moves the cursor over the three intensities, and picks one (cross) when
   the partner has its TP, else tells it has not; or leaves (triangle) */
static inline void STGTRAIN_pickIntensity(TrainSession *session) {
    PartnerTotals totals;
    s32 last;

    last = session->intensity;
    if (PAD_PRESSED(PAD_LEFT) || PAD_REPEATED(PAD_LEFT)) {
        if (--session->intensity < 0) {
            session->intensity = 0;
        }
    } else if (PAD_PRESSED(PAD_RIGHT) || PAD_REPEATED(PAD_RIGHT)) {
        if (++session->intensity >= 3) {
            session->intensity = 2;
        }
    }
    if (last != session->intensity) {
        SOUND.playSound(SOUND_MENU_MOVE);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        GAME.funcs.computeStats(GAME.funcs.getPartyMember(session->screen->partner), &totals);
        if (totals.fields.tp < STGTRAIN_intensityCosts[session->intensity]) {
            session->substate = 0xA;
        } else {
            session->substate = 0xF;
            session->screen->intensity = session->intensity;
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        session->substate = 0x32;
        session->step = 1;
    }
}

/* Once cross is pressed, hides the not-enough-points notice and closes its
   panel */
static inline void STGTRAIN_closeNotice(TrainSession *session, TrainSessionWindows *win) {
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_MENU_CONFIRM);
        win->notice->setVisible(win->notice, 0);
        STGTRAIN_state.startFade(&session->panels[7], 0);
        session->substate++;
    }
}

/* Hides the intensities, the cursor and the message, and closes their panels */
static inline void STGTRAIN_closeIntensities(TrainSession *session, TrainSessionWindows *win) {
    s32 j;

    session->cursorShown = 0;
    win->text[0]->setVisible(win->text[0], 0);
    for (j = 0; j < 3; j++) {
        win->intensities[j]->setVisible(win->intensities[j], 0);
    }
    session->panels[6].level = 0;
    STGTRAIN_state.startFade(&session->panels[5], 0);
    STGTRAIN_state.startFade(&session->panels[0], 0);
    session->substate++;
}

/* Once the question's panels are open, asks whether to train at the picked
   intensity, with the cursor on yes */
static inline void STGTRAIN_askToTrain(TrainSession *session, TrainSessionWindows *win) {
    STGTRAIN_state.updateFade(&session->panels[3]);
    STGTRAIN_state.updateFade(&session->panels[0]);
    if (STGTRAIN_state.updateFade(&session->panels[4])) {
        win->text[2]->setString(win->text[2], FILE_CACHE.load(STGTRAIN_TEXT), session->intensity + 0xE);
        win->text[0]->setString(win->text[0], FILE_CACHE.load(STGTRAIN_TEXT), 9);
        win->answers[0]->setString(win->answers[0], FILE_CACHE.load(STGTRAIN_TEXT), 0xA);
        win->answers[1]->setString(win->answers[1], FILE_CACHE.load(STGTRAIN_TEXT), 0xB);
        session->choice = 0;
        win->cursor->setPos(win->cursor, 0x94, 0x64);
        win->cursor->setVisible(win->cursor, 1);
        session->substate++;
    }
}

/* Moves the cursor between yes and no; yes pays the intensity's TP and
   starts the training, no (or triangle) goes back to the intensities */
static inline void STGTRAIN_answerToTrain(TrainSession *session, TrainSessionWindows *win) {
    PartnerStats *stats;
    s32 choice;

    choice = session->choice;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        session->choice = 0;
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        session->choice = 1;
    }
    if (choice != session->choice) {
        SOUND.playSound(SOUND_CURSOR);
        win->cursor->setPos(win->cursor, 0x94, session->choice * 16 + 0x64);
    } else if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        if (session->choice == 0) {
            session->substate = 0x19;
            stats = GAME.funcs.getPartnerStats(GAME.funcs.getPartyMember(session->screen->partner));
            stats->stats[STAT_TP] -= STGTRAIN_intensityCosts[session->intensity];
        } else {
            session->substate++;
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        session->substate++;
    }
}

/* Hides the question, its answers and the cursor, and closes their panels */
static inline void STGTRAIN_closeQuestion(TrainSession *session, TrainSessionWindows *win) {
    STGTRAIN_state.startFade(&session->panels[0], 0);
    STGTRAIN_state.startFade(&session->panels[4], 0);
    STGTRAIN_state.startFade(&session->panels[3], 0);
    win->text[2]->setVisible(win->text[2], 0);
    win->text[0]->setVisible(win->text[0], 0);
    win->answers[0]->setVisible(win->answers[0], 0);
    win->answers[1]->setVisible(win->answers[1], 0);
    win->cursor->setVisible(win->cursor, 0);
    session->substate++;
}

/* Starts the training: hides the question's message, answers and cursor,
   and closes their panels */
static inline void STGTRAIN_startTraining(TrainSession *session, TrainSessionWindows *win) {
    STGTRAIN_state.startFade(&session->panels[0], 0);
    win->text[0]->setVisible(win->text[0], 0);
    STGTRAIN_state.startFade(&session->panels[4], 0);
    win->answers[0]->setVisible(win->answers[0], 0);
    win->answers[1]->setVisible(win->answers[1], 0);
    win->cursor->setVisible(win->cursor, 0);
    session->substate++;
}

/* Ends the session once the training is over: hides the titles and closes
   their panels */
static inline void STGTRAIN_endSession(TrainSession *session, TrainSessionWindows *win) {
    win->text[1]->setVisible(win->text[1], 0);
    win->text[2]->setVisible(win->text[2], 0);
    session->panels[2].level = 0;
    session->panels[3].level = 0;
    STGTRAIN_state.startFade(&session->panels[1], 0);
    session->substate++;
}

/* Starts closing the session (triangle): hides its texts and closes its
   panels */
static inline void STGTRAIN_startClosing(TrainSession *session, TrainSessionWindows *win) {
    s32 k;

    session->cursorShown = 0;
    win->text[0]->setVisible(win->text[0], 0);
    for (k = 0; k < 3; k++) {
        win->intensities[k]->setVisible(win->intensities[k], 0);
    }
    win->text[1]->setVisible(win->text[1], 0);
    STGTRAIN_state.startFade(&session->panels[1], 0);
    STGTRAIN_state.startFade(&session->panels[2], 0);
    STGTRAIN_state.startFade(&session->panels[0], 0);
    STGTRAIN_state.startFade(&session->panels[6], 0);
    STGTRAIN_state.startFade(&session->panels[5], 0);
    session->substate++;
}

/*
 * Runs a training session: opens its panels, picks one of three
 * intensities (it costs STGTRAIN_intensityCosts's points of totals.fields.tp), asks to confirm
 * and closes, leaving the intensity in the screen's. The match depends on
 * each loop and each cursor's last value having a variable of its own (now
 * each in its own step): shared, they take other registers.
 */
void STGTRAIN_runSession(TrainSession *session, TrainSessionWindows *win) {
    switch (session->substate) {
    case 0:
    default:
        STGTRAIN_state.startFade(&session->panels[1], 1);
        session->substate++;
        break;
    case 1:
        STGTRAIN_showTrainingName(session, win);
        break;
    case 2:
        if (STGTRAIN_state.updateFade(&session->panels[2])) {
            STGTRAIN_state.startFade(&session->panels[0], 1);
            session->substate++;
        }
        break;
    case 3:
        if (STGTRAIN_state.updateFade(&session->panels[0])) {
            win->text[0]->setString(win->text[0], FILE_CACHE.load(STGTRAIN_TEXT), 8);
            STGTRAIN_state.startFade(&session->panels[5], 1);
            session->substate++;
        }
        break;
    case 4:
        if (STGTRAIN_state.updateFade(&session->panels[5])) {
            STGTRAIN_state.startFade(&session->panels[6], 1);
            session->substate++;
        }
        break;
    case 5:
        STGTRAIN_showIntensities(session, win);
        break;
    case 6:
        STGTRAIN_pickIntensity(session);
        break;
    case 0xA:
        session->cursorShown = 0;
        STGTRAIN_state.startFade(&session->panels[7], 1);
        session->substate++;
        break;
    case 0xB:
        if (STGTRAIN_state.updateFade(&session->panels[7])) {
            win->notice->setString(win->notice, FILE_CACHE.load(STGTRAIN_TEXT), 0x12);
            session->substate++;
        }
        break;
    case 0xC:
        STGTRAIN_closeNotice(session, win);
        break;
    case 0xD:
        if (STGTRAIN_state.updateFade(&session->panels[7])) {
            session->cursorShown = 1;
            session->substate = 6;
        }
        break;
    case 0xF:
        STGTRAIN_closeIntensities(session, win);
        break;
    case 0x10:
        STGTRAIN_state.updateFade(&session->panels[5]);
        if (STGTRAIN_state.updateFade(&session->panels[0])) {
            STGTRAIN_state.startFade(&session->panels[0], 1);
            STGTRAIN_state.startFade(&session->panels[4], 1);
            STGTRAIN_state.startFade(&session->panels[3], 1);
            session->substate++;
        }
        break;
    case 0x11:
        STGTRAIN_askToTrain(session, win);
        break;
    case 0x12:
        STGTRAIN_answerToTrain(session, win);
        break;
    case 0x13:
        STGTRAIN_closeQuestion(session, win);
        break;
    case 0x14:
        STGTRAIN_state.updateFade(&session->panels[3]);
        STGTRAIN_state.updateFade(&session->panels[0]);
        if (STGTRAIN_state.updateFade(&session->panels[4])) {
            STGTRAIN_state.startFade(&session->panels[0], 1);
            session->substate = 3;
        }
        break;
    case 0x19:
        STGTRAIN_startTraining(session, win);
        break;
    case 0x1A:
        STGTRAIN_state.updateFade(&session->panels[0]);
        if (STGTRAIN_state.updateFade(&session->panels[4])) {
            session->substate = 0x23;
        }
        break;
    case 0x1E:
        STGTRAIN_endSession(session, win);
        break;
    case 0x1F:
        if (STGTRAIN_state.updateFade(&session->panels[1])) {
            session->state = TASK_KILL;
        }
        break;
    case 0x23: /* a step that does nothing: its table entry leaves the switch */
        break;
    case 0x32:
        STGTRAIN_startClosing(session, win);
        break;
    case 0x33:
        STGTRAIN_state.updateFade(&session->panels[1]);
        STGTRAIN_state.updateFade(&session->panels[2]);
        STGTRAIN_state.updateFade(&session->panels[0]);
        STGTRAIN_state.updateFade(&session->panels[6]);
        if (STGTRAIN_state.updateFade(&session->panels[5]) && session->step != 0) {
            session->state = TASK_DONE;
        }
        break;
    }
}

/* A training session */
void STGTRAIN_updateSession(TrainSession *session, TrainSessionWindows *win) {
    switch (session->state) {
    case TASK_INIT:
    default:
        session->nextState(session);
        STGTRAIN_createSessionWindows(session, win);
        session->panels[6].duration = 8;
        session->panels[5].duration = 10;
        session->panels[4].duration = 10;
        session->panels[0].duration = 10;
        session->panels[1].duration = 10;
        session->panels[3].duration = 6;
        session->panels[2].duration = 6;
        session->panels[7].duration = 10;
        session->intensity = session->screen->intensity;
        break;
    case TASK_RUN:
        STGTRAIN_runSession(session, win);
        /* fallthrough */
    case TASK_DONE:
        STGTRAIN_drawSession(session);
    case TASK_KILL:
        break;
    }
}

/* Makes the session close (its substate 0x1E) */
void STGTRAIN_finishSession(TrainSession *session) {
    session->substate = 0x1E;
}

/* Creates a training session */
TrainSession *STGTRAIN_createSession(TrainScreen *screen) {
    TrainSession *session = createTask(STGTRAIN_updateSession, sizeof(TrainSession), sizeof(TrainSessionWindows));

    session->finish = STGTRAIN_finishSession;
    session->layerId = SCREEN_LAYER;
    session->depth = 6;
    session->screen = screen;
    return session;
}
