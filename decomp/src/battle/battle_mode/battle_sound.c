/* FIGHTSTG's battle sounds. */

#include "battle/battle_mode.h"

/* The battle sound's task: keys its sound off once its time is up */
void FIGHTSTG_updateBattleSound(BattleSound *task) {
    switch (task->state) {
    case TASK_INIT:
    case TASK_RUN:
    default:
        task->time -= FIGHTSTG_battle.frames;
        if (task->time <= 0) {
            SOUND.keyOff(task->sound, task->voice);
            task->setState(task, TASK_KILL);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* the sound ids of FIGHTSTG_playBattleSound, by index */
s32 FIGHTSTG_battleSounds[] = {
    SOUND_WIN_JINGLE, SOUND_MENU_CONFIRM, 0x40004, 0x4000A,
    0x4000B, 0x4000C, SOUND_INN_JINGLE, SOUND_RECOVERY,
    SOUND_MENU_CLOSE, SOUND_MENU_OPEN, 0x40017, 0x40011,
    0x4000F, 0x4001F, 0x40016, 0x40010,
    0x4000E, 0x40012, SOUND_SWITCH01, SOUND_SWITCH02,
    0x8004213E, 0x800421BF, 0xA0042240, 0x80042342,
    0x80042444, 0x8004293E, SOUND_COMCD103, 0x80042A40,
    0x40001, 0x80042B42, 0x80042C44, 0x80042CC5,
    0x80042D46, 0x80042DC7, 0x80042E48, 0xA0042F4A,
    SOUND_COMCD115, 0xA004303C, 0x800430BD, 0x8004313E,
    0xA00431BF, 0xA0043240, 0x800432C1, 0x80043342,
    0x800433C3, 0x800434C5, 0x80043546, 0xA00435C7,
    0x80043648, 0x8004374A, 0x800437CB, 0x8004383C,
    0x80043A40, 0xA0043BC3, 0xA0043C44, 0x800440BD,
    0x8004413E, 0x800441BF, 0x80044240, 0x800442C1,
    0x80044444, 0x800445C7, 0x80044648, SOUND_COMEX113,
    0x8004474A, 0x8004483C, 0x41180000, SOUND_SELECT,
    SOUND_MENU_CANCEL, SOUND_CURSOR, SOUND_COUNT, SOUND_SYSTEM05,
    SOUND_MENU_MOVE, 0x800454C4, 0x8004583C, 0x800458BD,
    0x8004593E, 0x800459BF, 0x80045A40, 0x80045AC1,
    0x80045B42, 0x80045BC3, 0x80045C44, 0x80045CC5,
    0x80045D46, 0x80045DC7, 0x8004603C, SOUND_CARD_001,
    0x8004613E, 0x20040006, 0, 0,
};
/* how the fight stage and WFIGHTMN read the battle table */
BattleTableEntry *(*FIGHTSTG_battleTableFunc)(s32 id) = FIGHTSTG_getBattleTableEntry;

/* Plays battle sound index (FIGHTSTG_battleSounds; 0x5A the battle's music,
   0x5B stops sound 0x20040006), for time frames when time is not 0 */
BattleSound *FIGHTSTG_playBattleSound(s32 index, s32 time) {
    s32 id;
    BattleSound *task;

    switch (index) {
    default:
        id = FIGHTSTG_battleSounds[index];
        break;
    case 0x5A:
        id = BATTLE_SETUP.music;
        break;
    case 0x5B:
        SOUND.stopSound(0x20040006);
        return NULL;
    }
    if (time == 0) {
        SOUND.playSound(id);
        return NULL;
    }
    task = createTask(FIGHTSTG_updateBattleSound, sizeof(BattleSound), 0);
    task->sound = id;
    task->voice = SOUND.playSound(id);
    task->time = time;
    return task;
}
