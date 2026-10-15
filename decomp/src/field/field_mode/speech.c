/* The message boxes and the talk boxes that follow an actor */

#include "field/field_mode.h"

/* Where a talk box goes: beside its actor's head on the screen, on the side
   its type says */
void FIELDSTG_getSpeechPos(Speech *task, Vec2 *out) {
    Vec2 pos;

    pos.x = task->actor->tile.x;
    pos.y = task->actor->tile.y;
    if (task->actor->key1 == 0xD6) {
        pos.y -= 0x15;
    }
    FIELDSTG_scriptHelpers[0](&pos);
    if (task->type == 2 || task->type == 3) {
        pos.x -= 0xB;
    } else {
        pos.x += 0xB;
    }
    if (task->type == 0 || task->type == 2) {
        pos.y -= 0x13;
    } else {
        pos.y -= 7;
    }
    *out = pos;
}

/* A speech's update: opens its message box or talk box, moves the talk box
   with its actor, and ends with it */
void FIELDSTG_updateSpeech(Speech *task, void **box) {
    Vec2 pos;
    Vec2 newPos;
    TalkBox *talkBox;

    switch (task->state) {
    case TASK_INIT:
    default:
        if (task->isMessage != 0) {
            *box = createMessageBox(FIELD_LAYER_TEXT, task->text, task->entry);
        } else {
            FIELDSTG_getSpeechPos(task, &pos);
            *box = createTalkBox(FIELD_LAYER_TEXT, pos.x, pos.y, task->text, task->entry, task->type);
        }
        task->nextState(task);
        break;
    case TASK_RUN:
        if (*box == NULL) {
            task->setState(task, TASK_KILL);
        } else if (task->isMessage == 0) {
            FIELDSTG_getSpeechPos(task, &newPos);
            talkBox = *box;
            talkBox->setPos(talkBox, newPos.x, newPos.y);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the speech of an event's text entry: a message box, or a talk box
   of a type */
Speech *FIELDSTG_createSpeech(Actor *actor, s32 entry, s32 type, s32 isMessage) {
    Speech *task = createTask(FIELDSTG_updateSpeech, sizeof(Speech), 4);

    task->actor = actor;
    task->entry = entry;
    task->type = type;
    task->isMessage = isMessage;
    task->text = FILE_CACHE.getEntry(FIELDSTG_state.eventText);
    return task;
}

/* Creates the talk box of an actor's text entry, on the side of the screen
   away from where it faces and the edges */
Speech *FIELDSTG_createTalk(Actor *actor, s32 entry) {
    Speech *task = createTask(FIELDSTG_updateSpeech, sizeof(Speech), 4);
    Vec2 pos;

    task->actor = actor;
    task->entry = entry;
    task->text = FILE_CACHE.load(FIELDSTG_state.textFile);
    pos = actor->tile;
    FIELDSTG_scriptHelpers[0](&pos);
    switch (actor->dir) {
        case 0:
        case 1:
        case 2:
        case 6:
        case 7:
        default:
            if (pos.x >= 0xA0) {
                task->type = 0;
            } else {
                task->type = 2;
            }
            break;
        case 3:
        case 4:
        case 5:
            if (pos.x >= 0xA0) {
                task->type = 1;
            } else {
                task->type = 3;
            }
            break;
    }
    switch (task->type) {
        case 0:
            if (pos.y < 0x79) {
                task->type = 1;
            }
            break;
        case 2:
            if (pos.y < 0x79) {
                task->type = 3;
            }
            break;
        case 1:
            if (pos.y >= 0xAC) {
                task->type = 0;
            }
            break;
        case 3:
            if (pos.y >= 0xAC) {
                task->type = 2;
            }
            break;
    }
    task->isMessage = 0;
    return task;
}
