#include "engine/game.h"

/* updateInnMenu's substates: each group runs in order, by substate++ */
enum InnSubstate {
    INN_OPEN_NAME, /* the inn's name and the money */
    INN_OPEN_QUESTION,
    INN_SHOW_QUESTION, /* the price and yes/no */
    INN_CHOOSE,
    INN_CLOSE_QUESTION = 10,
    INN_WAIT_QUESTION_CLOSED,
    INN_WAIT_NAME_CLOSED,
    INN_PAY = 20, /* the jingle and the fade to black */
    INN_SLEEP, /* pay and heal behind the black screen */
    INN_WAKE,
    INN_WAIT_AWAKE,
    INN_NO_MONEY = 30, /* the "not enough money" message */
    INN_NO_MONEY_WAIT,
    INN_NO_MONEY_CLOSE,
};

/* Starts opening or closing a menu panel, with its sound */
void innStartPanel(PanelAnim *panel, s32 open) {
    panel->active = 1;
    if (open) {
        SOUND.playSound(SOUND_MENU_OPEN);
        panel->level = 0;
        panel->step = ONE / panel->duration;
    } else {
        SOUND.playSound(SOUND_MENU_CLOSE);
        panel->level = ONE;
        panel->step = -((ONE / panel->duration) * 2);
    }
}

/* Moves a panel's level one frame on; 1 once it is fully open or closed */
s32 innUpdatePanel(PanelAnim *panel) {
    if (!panel->active) {
        return 1;
    }
    panel->level += panel->step;
    if (panel->step > 0) {
        if (panel->level > ONE) {
            panel->level = ONE;
            panel->active = 0;
            return 1;
        }
    } else if (panel->level < 0) {
        panel->level = 0;
        panel->active = 0;
        return 1;
    }
    return 0;
}

/* Restores HP, MP and status of the party (the inn) */
void healParty(void) {
    PartnerStats *p;
    s32 i;
    s32 j;
    s32 index;

    for (i = 0; i < PARTY_SIZE; i++) {
        index = GAME.funcs.getPartyMember(i);
        if (index >= 0) {
            p = GAME.funcs.getPartnerStats(index);
            p->stats[STAT_HP] = p->stats[STAT_MAX_HP];
            p->stats[STAT_MP] = p->stats[STAT_MAX_MP];
            for (j = 2; j >= 0; j--) {
                p->status[j] = 0;
            }
        }
    }
}

/* One inn per game mode */
typedef struct InnInfo {
    /* 0x0 */ s32 mode;
    /* 0x4 */ s16 string; /* in file 0x5D */
    /* 0x6 */ s16 price;
} InnInfo;

InnInfo INNS[] = {
    { 0x20A, 1, 8 },
    { 0x223, 2, 12 },
    { 0x22F, 3, 16 },
    { 0x238, 4, 32 },
    { 0x23F, 5, 36 },
    { 0x25B, 6, 48 },
    { 0x25D, 7, 44 },
    { 0x263, 8, 80 },
    { 0x26F, 9, 84 },
    { 0x26D, 10, 88 },
    { 0x279, 11, 52 },
    { 0x292, 2, 56 },
    { 0x29D, 12, 60 },
    { 0x2A5, 4, 68 },
    { 0x2AC, 13, 72 },
    { 0x2C4, 6, 80 },
    { 0x2C6, 14, 76 },
    { 0x2CB, 8, 84 },
    { 0x2D6, 15, 88 },
    { 0x249, 21, 44 },
    { 0x2B3, 21, 76 },
    { 0, 1, 1 }, /* the end of the list */
};

/* INN_CHOOSE: moves the cursor between yes and no; on yes pays if the party can,
   else shows the "not enough money" message; no or a cancel closes the question */
static inline void chooseInnAnswer(Inn *task, InnChildren *data) {
    s32 prev;

    prev = task->choice;
    if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
        task->choice = 0;
    } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
        task->choice = 1;
    }
    if (prev != task->choice) {
        SOUND.playSound(SOUND_CURSOR);
        data->cursor->setPos(data->cursor, 0xB8, task->choice * 16 + 0x5F);
        return;
    }
    if (PAD_PRESSED(PAD_CROSS)) {
        SOUND.playSound(SOUND_SELECT);
        if (task->choice != 0) {
            task->substate = INN_CLOSE_QUESTION;
        } else if (GAME.money >= INNS[task->inn].price * task->count) {
            task->substate = INN_PAY;
        } else {
            task->substate = INN_CLOSE_QUESTION;
            task->step = 1;
        }
    } else if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        task->substate = INN_CLOSE_QUESTION;
    }
}

/* Under the fade: hides the panels and windows, pays the night and heals the party */
static inline void sleepAtInn(Inn *task, InnChildren *data) {
    task->panels[0].level = 0;
    task->panels[1].level = 0;
    data->windows[0]->setVisible(data->windows[0], 0);
    data->windows[1]->setVisible(data->windows[1], 0);
    data->windows[2]->setVisible(data->windows[2], 0);
    data->windows[3]->setVisible(data->windows[3], 0);
    data->windows[4]->setVisible(data->windows[4], 0);
    data->windows[5]->setVisible(data->windows[5], 0);
    data->cursor->setVisible(data->cursor, 0);
    GAME.money -= INNS[task->inn].price * task->count;
    healParty();
}

/*
 * The inn's menu (INN_*): open the panels and ask (price x party size), then
 * close, pay and sleep, or show the "not enough money" message.
 */
void updateInnMenu(Inn *task, InnChildren *data) {
    switch (task->substate) {
    case INN_OPEN_NAME:
    default:
        innStartPanel(&task->panels[0], 1);
        task->substate++;
        break;
    case INN_OPEN_QUESTION:
        if (innUpdatePanel(&task->panels[0])) {
            data->windows[0]->setString(data->windows[0], FILE_CACHE.load(TEXT_FILE(TEXT_INN_NAMES)), INNS[task->inn].string);
            data->windows[1]->setNumber(data->windows[1], 0, GAME.money);
            data->windows[1]->setRightAlign(data->windows[1], 1);
            data->windows[2]->setString(data->windows[2], FILE_CACHE.load(TEXT_FILE(TEXT_INN_NAMES)), 0x10);
            innStartPanel(&task->panels[1], 1);
            task->substate++;
        }
        break;
    case INN_SHOW_QUESTION:
        if (innUpdatePanel(&task->panels[1])) {
            data->windows[3]->setString(data->windows[3], FILE_CACHE.load(TEXT_FILE(TEXT_INN_NAMES)), 0x11);
            data->windows[3]->setNumber(data->windows[3], 1, INNS[task->inn].price);
            data->windows[4]->setString(data->windows[4], FILE_CACHE.load(TEXT_FILE(TEXT_INN_NAMES)), 0x12);
            data->windows[5]->setString(data->windows[5], FILE_CACHE.load(TEXT_FILE(TEXT_INN_NAMES)), 0x13);
            data->cursor->setVisible(data->cursor, 1);
            task->substate++;
        }
        break;
    case INN_CHOOSE:
        chooseInnAnswer(task, data);
        break;
    case INN_CLOSE_QUESTION:
        innStartPanel(&task->panels[1], 0);
        data->windows[3]->setVisible(data->windows[3], 0);
        data->windows[4]->setVisible(data->windows[4], 0);
        data->windows[5]->setVisible(data->windows[5], 0);
        data->cursor->setVisible(data->cursor, 0);
        task->substate++;
        break;
    case INN_WAIT_QUESTION_CLOSED:
        if (innUpdatePanel(&task->panels[1])) {
            if (task->step != 0) {
                innStartPanel(&task->panels[2], 1);
                task->substate = INN_NO_MONEY;
            } else {
                innStartPanel(&task->panels[0], 0);
                data->windows[0]->setVisible(data->windows[0], 0);
                data->windows[1]->setVisible(data->windows[1], 0);
                data->windows[2]->setVisible(data->windows[2], 0);
                task->substate++;
            }
        }
        break;
    case INN_WAIT_NAME_CLOSED:
        if (innUpdatePanel(&task->panels[0])) {
            task->state = TASK_KILL;
        }
        break;
    case INN_PAY:
        task->music = SOUND.music;
        task->time = GFX.funcs.getTime();
        SOUND.playSound(SOUND_INN_JINGLE);
        data->fade = createScreenFade(task->layerId);
        data->fade->start(data->fade, 0, 0x14);
        task->substate++;
        break;
    case INN_SLEEP:
        if (data->fade->state == TASK_DONE) {
            sleepAtInn(task, data);
            task->substate++;
        }
        break;
    case INN_WAKE:
        if (GFX.funcs.getTime() - task->time > 0xF0) {
            data->fade->start(data->fade, 1, 0x14);
            task->substate++;
        }
        break;
    case INN_WAIT_AWAKE:
        if (data->fade->state == TASK_DONE) {
            SOUND.playSound(task->music);
            task->state = TASK_KILL;
        }
        break;
    case INN_NO_MONEY:
        if (innUpdatePanel(&task->panels[2])) {
            data->windows[3]->setString(data->windows[3], FILE_CACHE.load(TEXT_FILE(TEXT_INN_NAMES)), 0x14);
            task->step = 0;
            task->substate++;
        }
        break;
    case INN_NO_MONEY_WAIT:
        if (PAD_PRESSED(PAD_CROSS)) {
            SOUND.playSound(SOUND_MENU_CONFIRM);
            data->windows[3]->setVisible(data->windows[3], 0);
            innStartPanel(&task->panels[2], 0);
            task->substate++;
        }
        break;
    case INN_NO_MONEY_CLOSE:
        if (innUpdatePanel(&task->panels[2])) {
            innStartPanel(&task->panels[1], 1);
            task->substate = INN_SHOW_QUESTION;
        }
        break;
    }
}

/* The inn task: creates its windows, then runs the menu and draws the panels at their levels */
void updateInn(Inn *task, InnChildren *data) {
    SpriteDrawer obj;
    s32 i;
    s32 n;
    s32 id;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        id = GAME.funcs.getMode();
        for (n = 0; INNS[n].mode != 0; n++) {
            if (id == INNS[n].mode) {
                break;
            }
        }
        task->inn = n;
        for (i = 0; i < PARTY_SIZE; i++) {
            if (GAME.funcs.getPartyMember(i) >= 0) {
                task->count++;
            }
        }
        data->windows[0] = createTextWindow(task->layerId, 1, 0x1D, 0x14);
        data->windows[1] = createTextWindow(task->layerId, 3, 0x117, 0x17);
        data->windows[2] = createTextWindow(task->layerId, 3, 0x11A, 0x17);
        data->windows[3] = createTextWindow(task->layerId, 1, 0x79, 0x34);
        data->windows[4] = createTextWindow(task->layerId, 1, 0xC5, 0x5F);
        data->windows[5] = createTextWindow(task->layerId, 1, 0xC5, 0x6F);
        data->cursor = createCursor(task->layerId, 0, 0xB8, 0x5F);
        data->cursor->setVisible(data->cursor, 0);
        task->panels[0].duration = 10;
        task->panels[1].duration = 10;
        task->panels[2].duration = 10;
        break;
    case TASK_RUN:
        updateInnMenu(task, data);
        initSpriteDrawer(&obj);
        obj.setTexture(0x140, 0);
        obj.setLayerId(task->layerId, task->depth);
        obj.setFollowScroll(0);
        if (task->panels[0].level != 0) {
            if (task->panels[0].level != ONE) {
                obj.setScale(task->panels[0].level, ONE, ONE);
                obj.setPivot(0x57, 0x19);
            }
            obj.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x41, 0x16, 0x12);
            if (task->panels[0].level != ONE) {
                obj.setPivot(0x140, 0x18);
            }
            obj.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x42, 0xD6, 0xF);
        }
        if (task->panels[1].level != 0) {
            if (task->panels[1].level != ONE) {
                obj.setScale(task->panels[1].level, ONE, ONE);
                obj.setPivot(0x140, 0x41);
            }
            obj.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x43, 0x4C, 0x2E);
            if (task->panels[1].level != ONE) {
                obj.setPivot(0x140, 0x6D);
            }
            obj.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x44, 0xAF, 0x59);
        }
        if (task->panels[2].level != 0) {
            if (task->panels[2].level != ONE) {
                obj.setScale(task->panels[2].level, ONE, ONE);
                obj.setPivot(0x140, 0x41);
            }
            obj.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x43, 0x4C, 0x2E);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Starts the inn of the current game mode on a layer */
Inn *createInn(s32 layerId) {
    Inn *task = createTask(updateInn, 0xA0, sizeof(InnChildren));

    task->layerId = layerId;
    task->depth = 1;
    return task;
}
