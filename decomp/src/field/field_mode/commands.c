/* The field commands (FIELD_COMMAND_*) that the stages' event scripts send
   through script command 813: the partners halted, the player icon's
   animations, map objects shown or hidden, the camera's shake and sounds.
   The first object of FIELDSTG.PRO (see data/fieldstg.c). */

#include "field/field_mode.h"

/* The task of script command 813: on substate 1 it stops the partners
   following */
void FIELDSTG_updateCommandTask(Task *task) {
    switch (task->state) {
    case TASK_INIT:
    case TASK_RUN:
    default:
        if (task->substate == 1) {
            FIELDSTG_haltPartners();
            task->setSubstate(task, 0);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* The commands that hide or show a group of the map objects (by their anim) */
static inline void showObjectGroups(s32 command) {
    StageTile *object;

    switch (command) {
    case FIELD_COMMAND_HIDE_OBJECTS(0) ... FIELD_COMMAND_HIDE_OBJECTS(FIELD_OBJECT_GROUPS - 1):
        for (object = FIELDSTG_state.objects; object->margin != 0; object++) {
            if (object->anim == command - FIELD_COMMAND_HIDE_OBJECTS(0) + FIELD_OBJECT_GROUP_ANIM) {
                object->visible = 0;
            }
        }
        break;
    case FIELD_COMMAND_SHOW_OBJECTS(0) ... FIELD_COMMAND_SHOW_OBJECTS(FIELD_OBJECT_GROUPS - 1):
        for (object = FIELDSTG_state.objects; object->margin != 0; object++) {
            if (object->anim == command - FIELD_COMMAND_SHOW_OBJECTS(0) + FIELD_OBJECT_GROUP_ANIM) {
                object->visible = 1;
            }
        }
        break;
    }
}

/* The commands that play a sound */
static inline void playCommandSound(s32 command) {
    switch (command) {
    case FIELD_COMMAND_PLAY_INFO_SIG:
        SOUND.playSound(SOUND_INFO_SIG);
        break;
    case FIELD_COMMAND_PLAY_WEAR_OFF:
        SOUND.playSound(SOUND_WEAR_OFF);
        break;
    case FIELD_COMMAND_PLAY_DEMO_BGM:
        SOUND.playSound(SOUND_DEMO_BGM);
        break;
    case FIELD_COMMAND_PLAY_SE000002:
        SOUND.playSound(SOUND_SE000002);
        break;
    case FIELD_COMMAND_PLAY_BEAM_SHT:
        SOUND.playSound(SOUND_BEAM_SHT);
        break;
    case FIELD_COMMAND_PLAY_SWITCH02:
        SOUND.playSound(SOUND_SWITCH02);
        break;
    case FIELD_COMMAND_PLAY_MASK_SET:
        SOUND.playSound(SOUND_MASK_SET);
        break;
    case FIELD_COMMAND_PLAY_BM_ERASE:
        SOUND.playSound(SOUND_BM_ERASE);
        break;
    case FIELD_COMMAND_PLAY_LD_ERASE:
        SOUND.playSound(SOUND_LD_ERASE);
        break;
    case FIELD_COMMAND_PLAY_TRAP_OFF:
        SOUND.playSound(SOUND_TRAP_OFF);
        break;
    case FIELD_COMMAND_PLAY_SAVEDEMO:
        SOUND.playSound(SOUND_SAVEDEMO);
        break;
    case FIELD_COMMAND_PLAY_SWITCH03:
        SOUND.playSound(SOUND_SWITCH03);
        break;
    case FIELD_COMMAND_PLAY_SN_ENTRY:
        SOUND.playSound(SOUND_SN_ENTRY);
        break;
    case FIELD_COMMAND_PLAY_SN_ERASE:
        SOUND.playSound(SOUND_SN_ERASE);
        break;
    case FIELD_COMMAND_PLAY_TELEPORT:
        SOUND.playSound(SOUND_TELEPORT);
        break;
    case FIELD_COMMAND_PLAY_BULB_003:
        SOUND.playSound(SOUND_BULB_003);
        break;
    case FIELD_COMMAND_PLAY_GONDRA_S:
        SOUND.playSound(SOUND_GONDRA_S);
        break;
    case FIELD_COMMAND_PLAY_PIYOPIYO:
        SOUND.playSound(SOUND_PIYOPIYO);
        break;
    case FIELD_COMMAND_PLAY_COMCD103:
        SOUND.playSound(SOUND_COMCD103);
        break;
    case FIELD_COMMAND_PLAY_COMCD201:
        SOUND.playSound(SOUND_COMCD201);
        break;
    case FIELD_COMMAND_PLAY_COMCD111:
        SOUND.playSound(SOUND_COMCD111);
        break;
    case FIELD_COMMAND_PLAY_SWITCH01:
        SOUND.playSound(SOUND_SWITCH01);
        break;
    }
}

/* The commands that play a sound that goes on until a stop command keys it
   off (FIELDSTG_heldVoice) */
static inline void playHeldSound(s32 command) {
    switch (command) {
    case FIELD_COMMAND_PLAY_GAYALOOP:
        FIELDSTG_heldVoice = SOUND.playSound(SOUND_GAYALOOP);
        break;
    case FIELD_COMMAND_PLAY_PLAYER11:
        FIELDSTG_heldVoice = SOUND.playSound(SOUND_PLAYER11);
        break;
    case FIELD_COMMAND_PLAY_BEAM_HIT:
        FIELDSTG_heldVoice = SOUND.playSound(SOUND_BEAM_HIT);
        break;
    case FIELD_COMMAND_PLAY_COMCD115:
        FIELDSTG_heldVoice = SOUND.playSound(SOUND_COMCD115);
        break;
    }
}

/* The commands that stop that sound */
static inline void stopHeldSound(s32 command) {
    switch (command) {
    case FIELD_COMMAND_STOP_GAYALOOP:
        SOUND.keyOff(SOUND_GAYALOOP, FIELDSTG_heldVoice);
        break;
    case FIELD_COMMAND_STOP_PLAYER11:
        SOUND.keyOff(SOUND_PLAYER11, FIELDSTG_heldVoice);
        break;
    case FIELD_COMMAND_STOP_COMCD115:
        SOUND.keyOff(SOUND_COMCD115, FIELDSTG_heldVoice);
        break;
    case FIELD_COMMAND_STOP_BEAM_HIT:
        SOUND.keyOff(SOUND_BEAM_HIT, FIELDSTG_heldVoice);
        break;
    }
}

/*
 * Runs a field command (FIELD_COMMAND_*) of an event script: halts the
 * partners through the command task, sets the player icon's substate, hides
 * or shows a group of map objects, shakes the camera, searches the event
 * spot, or plays a sound; four sounds are held, to be keyed off by a later
 * command.
 */
void FIELDSTG_handleFieldCommand(Task *task, s32 command) {
    Task *icon;
    s32 n;

    if (task == NULL) {
        return;
    }
    n = 0;
    if (command == FIELD_COMMAND_HALT_PARTNERS) {
        task->setSubstate(task, 1);
    }
    switch (command) {
    case FIELD_COMMAND_ICON3:
        n++;
        /* fallthrough */
    case FIELD_COMMAND_ICON2:
        n++;
        /* fallthrough */
    case FIELD_COMMAND_ICON1:
        n++;
        icon = TASK_REGISTRY.funcs.find(FIELD_TASK_ICON, -1, -1);
        icon->setSubstate(icon, n);
        break;
    }
    showObjectGroups(command);
    switch (command) {
    case FIELD_COMMAND_SHAKE_CAMERA:
        FIELDSTG_shakeCamera(1);
        break;
    case FIELD_COMMAND_STOP_CAMERA_SHAKE:
        FIELDSTG_shakeCamera(0);
        break;
    }
    if (command == FIELD_COMMAND_SEARCH_EVENT_SPOT) {
        FIELDSTG_searchEventSpot();
    }
    playCommandSound(command);
    playHeldSound(command);
    stopHeldSound(command);
}

/* Creates the task of script command 813 (the create of
   FIELDSTG_scriptCommands) */
void FIELDSTG_startCommandTask(void) {
    createTaskWithId(FIELDSTG_updateCommandTask, sizeof(Task), 0, FIELD_TASK_COMMANDS);
}
