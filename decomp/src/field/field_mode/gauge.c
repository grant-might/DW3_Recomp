/* The gauge game */

#include "field/field_mode.h"

/* The cursor goes back and forth over the gauge at the game's speed */
static inline void moveGaugeCursor(GaugeGame *task) {
    if (task->back) {
        task->cursor -= task->speed;
        if (task->cursor <= 0) {
            task->cursor = 0;
            task->back = 0;
        }
    } else {
        task->cursor += task->speed;
        if (task->cursor >= GAUGE_CURSOR_MAX) {
            task->cursor = GAUGE_CURSOR_MAX;
            task->back = 1;
        }
    }
}

/*
 * Draws the four frames sprite (0x60-0x63) at pos, next to the player, then
 * the gauge: its cursor, its row of cells and its frame
 */
static inline void drawGauge(GaugeGame *task) {
    SpriteDrawer drawer;
    SpriteDrawer gauge;
    void *sprites;
    void *gaugeSprites;

    sprites = FILE_CACHE.getEntry(FIELD_SPRITES_FILE << 16);
    initSpriteDrawer(&drawer);
    drawer.setLayerId(FIELD_LAYER_MAP, 4);
    drawer.setTexture(FIELD_SPRITES_X, FIELD_SPRITES_Y);
    drawer.draw(sprites, GFX.funcs.getTime() % 48 / 12 + 0x60, task->pos.x, task->pos.y);
    gaugeSprites = FILE_CACHE.getEntry((FIELD_SPRITES_FILE << 16) | 1);
    initSpriteDrawer(&gauge);
    gauge.setLayerId(FIELD_LAYER_MAP, 0);
    gauge.setTexture(FIELD_SPRITES2_X, FIELD_SPRITES_Y);
    gauge.setFollowScroll(0);
    gauge.draw(gaugeSprites, GAUGE_CURSOR_FRAME, (task->cursor >> 8) + GAUGE_X, GAUGE_Y);
    gauge.draw(gaugeSprites, task->row + GAUGE_ROW_FRAMES, GAUGE_X, GAUGE_Y);
    gauge.draw(gaugeSprites, GAUGE_FRAME, GAUGE_X, GAUGE_Y);
}

/* A gauge game: a cursor runs back and forth along one of the gauge rows until
 * cross is pressed, then slows down and stops; the cell it stops on (2 bits)
 * fails (0) or calls FIELDSTG_battleFuncs.startEventBattle with 4 (1) or 7 (2). In Europe the rows are
 * random only while GAME.randomGauges lasts, then it is GAUGE_EMPTY_ROW. The match
 * depends on the cell read and shifted as two statements. */
void FIELDSTG_runGauge(GaugeGame *task) {
    u8 cell;
    s32 index;
    s32 shift;

    switch (task->state) {
    case TASK_INIT:
    default:
#if VERSION_EU || VERSION_JP
        if (GAME.randomGauges > 0) {
            task->row = RANDOM.next() & (GAUGE_ROWS - 1);
            GAME.randomGauges--;
        } else {
            task->row = GAUGE_EMPTY_ROW;
        }
#else
        task->row = RANDOM.next() & (GAUGE_ROWS - 1);
#endif
        task->speed = 0x100;
        task->nextState(task);
        /* fallthrough */
    case TASK_RUN:
        switch (task->substate) {
        case 0:
        default:
            task->step += GFX.funcs.getFrameTime();
            if (task->step > GAUGE_START_DELAY) {
                task->nextSubstate(task);
            }
            break;
        case 1:
            switch (task->step) {
            case GAUGE_RUNNING:
            default:
                if (PAD.getPressed(0) & (1 << PAD_CROSS)) {
                    if (!(RANDOM.next() & 3)) {
                        task->setStep(task, GAUGE_STOPPING_SLOWLY);
                    } else {
                        task->setStep(task, GAUGE_STOPPING);
                    }
                    SOUND.playSound(SOUND_MENU_MOVE);
                }
                break;
            case GAUGE_STOPPING:
                task->speed -= 0x10;
                if (task->speed == 0) {
                    task->setStep(task, GAUGE_STOPPED);
                }
                break;
            case GAUGE_STOPPING_SLOWLY:
                task->speed -= 4;
                if (task->speed == 0) {
                    task->setStep(task, GAUGE_STOPPED);
                }
                break;
            case GAUGE_STOPPED:
#if VERSION_EU || VERSION_JP
                index = task->cursor >> 10;
                shift = (task->cursor >> 7) & 6;
                cell = FIELDSTG_gaugeRows[task->row][index];
                cell = (cell >> shift) & 3;
                if (task->counter < GAUGE_RESULT_DELAY) {
                    if (task->counter == 0 && cell == 1) {
                        SOUND.playSound(SOUND_SYSTEM05);
                    }
                    task->counter += GFX.funcs.getFrameTime();
                    break;
                }
#else
                task->counter += GFX.funcs.getFrameTime();
                if (task->counter < GAUGE_RESULT_DELAY) {
                    break;
                }
                index = task->cursor >> 10;
                shift = (task->cursor >> 7) & 6;
                cell = FIELDSTG_gaugeRows[task->row][index];
                cell = (cell >> shift) & 3;
#endif
                switch (cell) {
                case 0:
                default:
                    task->setState(task, TASK_KILL);
                    break;
                case 1:
                    FIELDSTG_battleFuncs.startEventBattle(4);
                    task->setState(task, TASK_DONE);
                    break;
                case 2:
                    FIELDSTG_battleFuncs.startEventBattle(7);
                    task->setState(task, TASK_DONE);
                    break;
                }
                break;
            }
            moveGaugeCursor(task);
            drawGauge(task);
            break;
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the gauge game at a tile */
GaugeGame *FIELDSTG_createGauge(Vec2 pos) {
    GaugeGame *task = createTask(FIELDSTG_runGauge, sizeof(GaugeGame), 0);

    task->pos = pos;
    return task;
}
