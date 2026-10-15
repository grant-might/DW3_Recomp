#include "engine/game.h"

/* Each mode's overlay (mode >> 8): its entry point and its file */
Task *(*MODE_ENTRY_POINTS[23])(void) = {
    NULL, NULL, FIELDSTG_start, FIELDSTG_start,
    STCRDDEK_start, STPLNMET_start, FIGHTSTG_start, CARDGAME_start,
    SOUNDTST_start, SHOCKTST_start, STGTRAIN_start, STDGNAME_start,
    STGMCARD_start, STGDGLAB_createScene, STDWTITL_start, STITSHOP_start,
    STSTATUS_start, NULL, STCRDABM_start, STCRDSHP_start,
    STFGTREP_start, STAGSLCT_start, CNTY_SEL_start,
};
s32 MODE_OVERLAY_FILES[23] = {
    0, 0, FILE_FIELDSTG, FILE_FIELDSTG,
    FILE_STCRDDEK, FILE_STPLNMET, FILE_FIGHTSTG, FILE_CARDGAME,
    FILE_SOUNDTST, FILE_SHOCKTST, FILE_STGTRAIN, FILE_STDGNAME,
    FILE_STGMCARD, FILE_STGDGLAB, FILE_STDWTITL, FILE_STITSHOP,
    FILE_STSTATUS, 0, FILE_STCRDABM, FILE_STCRDSHP,
    FILE_STFGTREP, FILE_STAGSLCT, FILE_CNTY_SEL,
};

OverlayLoader OVERLAY_LOADER = {0, 0, loadModeOverlay, loadSubOverlay};

/*
 * The root task (created by main): loads the overlay of the current game
 * mode and calls its entry point, which returns the mode's top task. Once a
 * new mode is requested it dies, and main creates it again.
 */
void updateModeTask(Task *task, Task **children) {
    switch (task->state) {
    case 0:
    default:
        OVERLAY_LOADER.loadModeOverlay();
        children[0] = MODE_ENTRY_POINTS[GAME.funcs.getMode() >> 8]();
        task->nextState(task);
        break;
    case 1:
        if (GAME.funcs.isModeChangePending() != 0) {
            task->setState(task, TASK_KILL);
        }
        break;
    case 2:
    case 3:
        break;
    }
}

/* Creates the task that runs the current mode (updateModeTask) */
void *createModeTask(void) {
    return createTask(updateModeTask, sizeof(Task), 4);
}

/* Copies the overlay of the current mode (mode >> 8) to OVERLAY_ADDRESS */
void loadModeOverlay(void) {
    s32 id = GAME.funcs.getMode() >> 8;
    u8 *src;
    void *dst;

    if (OVERLAY_LOADER.mode != id) {
        OVERLAY_LOADER.mode = id;
        OVERLAY_LOADER.subOverlay = -1;
        src = FILE_CACHE.load(MODE_OVERLAY_FILES[id]);
        dst = OVERLAY_ADDRESS;
        memcpy(dst, src, FILE_TABLE.getSectorCount(MODE_OVERLAY_FILES[id]) * CD_SECTOR_SIZE);
    }
}

/* Copies a file to the second overlay area (SUB_OVERLAY_ADDRESS) */
void loadSubOverlay(s32 id) {
    u8 *src;
    void *dst;

    if (OVERLAY_LOADER.subOverlay != id) {
        OVERLAY_LOADER.subOverlay = id;
        src = FILE_CACHE.load(id);
        dst = SUB_OVERLAY_ADDRESS;
        memcpy(dst, src, FILE_TABLE.getSectorCount(id) * CD_SECTOR_SIZE);
    }
}
