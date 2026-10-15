#include "engine/game.h"
#include "field/field_map.h"

/* Clears the save data and sets up a new game */
void newGame(void) {
    HEAP.zero(&GAME, GAME_SAVE_SIZE);
#if VERSION_US || VERSION_JP
    GAME.mode = MODE_OPENING;
    GAME.nextMode = MODE_OPENING;
#elif VERSION_EU
    GAME.mode = MODE_COUNTRY_SELECT;
    GAME.nextMode = MODE_COUNTRY_SELECT;
#endif
    GAME.digivolveDemo = 1;
    GAME.countdown[0] = 1;
    GAME.countdown[1] = 8;
    GAME.countdown[3] = 60;
    GAME.modeArg = 0;
    GAME.countdown[2] = 0;
    GAME.unusedC = -1;
    initNewGameData();
    GAME.battleSteps = (RANDOM.next() & 0x1FF) + 0x200;
}

/* Makes the requested mode current (main calls it before recreating the mode task) */
void commitMode(void) {
    s32 prev;

    if (GAME.nextMode != 0) {
        prev = GAME.mode;
        GAME.mode = GAME.nextMode;
        GAME.nextMode = 0;
        GAME.prevMode = prev;
    }
}

/* The game mode before the current one */
s32 getPrevMode(void) {
    return GAME.prevMode;
}

/* The current game mode */
s32 getMode(void) {
    return GAME.mode;
}

/* The argument the current mode was requested with */
s32 getModeArg(void) {
    return GAME.modeArg;
}

/* Asks for a mode change, applied when main recreates the mode task */
void requestMode(s32 mode, s32 arg) {
    GAME.nextMode = mode;
    GAME.modeArg = arg;
}

/* Whether a mode change was requested */
s32 isModeChangePending(void) {
    return GAME.nextMode != 0;
}

/* The save data, the game mode and the methods */
GameState GAME = {
    .funcs = {
        newGame, commitMode, getMode, getModeArg, requestMode, isModeChangePending, getPrevMode,
        getPartyMember, setParty, addCards, giveStarterDeck, getPartyPartner, setStat, addStat,
        computeStats, getPartnerSlots, setPartnerSlots, listPartnerEntries,
        addPartnerEntry, getPartnerEntry, setPartnerEntry, getPartnerStats,
        resetPlayTime, updatePlayTime,
    },
};
