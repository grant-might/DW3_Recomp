/* The yes/no questions of the story */

#include "field/field_mode.h"

/*
 * A yes/no question of the story (FIELDSTG_askChoice0 to FIELDSTG_askChoice15
 * make one per type): asks FIELDSTG_choices[type]'s question and starts the
 * event of the answer.
 */
void FIELDSTG_runChoice(ChoiceTask *task, ChoiceChildren *children) {
    SpriteDrawer drawer;
    s32 prev;
    s32 i;
    s32 j;
    s32 k;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        task->tween.duration = 10;
        for (i = 0; i < 2; i++) {
            children->options[i] = createTextWindow(FIELD_LAYER_MAP, 1, 0x1C, 0xBE + i * 14);
            children->options[i]->setDepth(children->options[i], 1);
        }
        children->cursor = createCursor(FIELD_LAYER_MAP, 1, 0x12, 0xBE);
        children->cursor->setVisible(children->cursor, 0);
        children->title = createTextWindow(FIELD_LAYER_MAP, 1, 0x12, 0xB0);
        break;
    case TASK_RUN:
        switch (task->substate) {
        case 0:
        default:
            FIELDSTG_tweenStart(&task->tween, 1);
            task->substate++;
            break;
        case 1:
            if (FIELDSTG_tweenUpdate(&task->tween)) {
                for (j = 0; j < 2; j++) {
                    children->options[j]->setString(children->options[j], FILE_CACHE.getEntry(FIELDSTG_choices[task->type].text + (TEXT_FILE(1) << 16)), j + 2);
                }
                children->cursor->setVisible(children->cursor, 1);
                children->title->setString(children->title, FILE_CACHE.getEntry(FIELDSTG_choices[task->type].text + (TEXT_FILE(1) << 16)), 1);
                task->substate++;
            }
            break;
        case 2:
            prev = task->selection;
            if (PAD_PRESSED(PAD_UP) || PAD_REPEATED(PAD_UP)) {
                if (--task->selection < 0) {
                    task->selection = 0;
                }
            } else if (PAD_PRESSED(PAD_DOWN) || PAD_REPEATED(PAD_DOWN)) {
                task->selection++;
                if (task->selection > 1) {
                    task->selection = 1;
                }
            }
            if (prev != task->selection) {
                SOUND.playSound(SOUND_CURSOR);
                children->cursor->setPos(children->cursor, 0x12, task->selection * 14 + 0xBE);
                break;
            }
            if (PAD_PRESSED(PAD_CROSS)) {
                SOUND.playSound(SOUND_SELECT);
                task->substate = 10;
            }
            break;
        case 3:
            children->event = FIELDSTG_startEvent(task->selection == 0 ? FIELDSTG_choices[task->type].events[0] : FIELDSTG_choices[task->type].events[1]);
            task->substate++;
            break;
        case 4:
            if (children->event == NULL) {
                task->state = TASK_KILL;
            }
            break;
        case 10:
            for (k = 0; k < 2; k++) {
                children->options[k]->setVisible(children->options[k], 0);
            }
            children->cursor->setVisible(children->cursor, 0);
            children->title->setVisible(children->title, 0);
            FIELDSTG_tweenStart(&task->tween, 0);
            task->substate++;
            break;
        case 11:
            if (FIELDSTG_tweenUpdate(&task->tween)) {
                task->substate = 3;
            }
            break;
        }
        initSpriteDrawer(&drawer);
        drawer.setLayerId(FIELD_LAYER_MAP, 2);
        drawer.setTexture(FIELD_MENU_SPRITES_X, 0);
        drawer.setFollowScroll(0);
        if (task->tween.level != 0) {
            if (task->tween.level != 0x1000) {
                drawer.setScale(task->tween.level, 0x1000, 0x1000);
                drawer.setPivot(0, 0xC3);
            }
            drawer.draw(FILE_CACHE.getEntry(FILE_MENU_SPRITES << 16), 0x45, 0, 0xAC);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Asks question n of FIELDSTG_choices: the start of the FieldEvent before the
   events of its answers (FIELDSTG_events) */
#define ASK_CHOICE(n)                                                                                  \
    void *FIELDSTG_askChoice##n(void) {                                                                \
        ChoiceTask *task = createTask(FIELDSTG_runChoice, sizeof(ChoiceTask), sizeof(ChoiceChildren)); \
        task->type = n;                                                                                \
        return task;                                                                                   \
    }

ASK_CHOICE(0)
ASK_CHOICE(1)
ASK_CHOICE(2)
ASK_CHOICE(3)
ASK_CHOICE(4)
ASK_CHOICE(5)
ASK_CHOICE(6)
ASK_CHOICE(7)
ASK_CHOICE(8)
ASK_CHOICE(9)
ASK_CHOICE(10)
ASK_CHOICE(11)
ASK_CHOICE(12)
ASK_CHOICE(13)
ASK_CHOICE(14)
ASK_CHOICE(15)
