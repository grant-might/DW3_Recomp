/* CARDGAME's table views: browsing the played cards and picking one. */

#include "cardgame/card_battle.h"

/* Starts CARDGAME_viewTable */
void CARDGAME_startViewTable(CardBattle *battle, CardScreen *screen) {
    battle->effectStep.nextState = 1;
}

/* The table's height: 8 per card of the longer row of slots (at least 3), and
   14 */
s32 CARDGAME_getTableHeight(CardBattle *battle, CardScreen *screen) {
    s32 n;

    if (battle->players[0].slotCount > battle->players[1].slotCount) {
        n = battle->players[0].slotCount;
    } else {
        n = battle->players[1].slotCount;
    }
    if (n < 3) {
        n = 3;
    }
    return n * 8 + 14;
}

/* Moves the highlight of a row of cards (kind 0, 1 or 2) by delta */
void CARDGAME_moveTableHighlight(CardBattle *battle, CardScreen *screen, s32 kind, s32 delta) {
    s32 offset = 0;

    SOUND.playSound(SOUND_MENU_MOVE);
    switch (kind) {
    case 1:
        offset = 6;
        break;
    case 2:
        offset = 12;
        break;
    }
    screen->sprites[offset + battle->effectStep.cursor].highlight &= ~1;
    screen->sprites[offset + battle->effectStep.cursor].moving = 0;
    battle->effectStep.cursor += delta;
    screen->sprites[offset + battle->effectStep.cursor].highlight |= 1;
    screen->sprites[offset + battle->effectStep.cursor].moving = 1;
}

/* Closes the card info's four windows */
static inline void closeTableWindows(CardScreen *screen) {
    screen->closeWindow(screen, 4);
    screen->closeWindow(screen, 1);
    screen->closeWindow(screen, 2);
    screen->closeWindow(screen, 3);
}

/* Starts showing a row of cards (0, 1 or 2) and its windows */
static inline void startTableRow(CardBattle *battle, CardScreen *screen, s32 row) {
    CARDGAME_clearCardInfo(battle, screen);
    battle->effectStep.vars[0] = 0;
    battle->effectStep.time = 0;
    battle->effectStep.vars[3] = row;
}

/* Leaves a row of cards (its first sprite at offset): unhighlights its card
   and closes the windows */
static inline void leaveTableRow(CardBattle *battle, CardScreen *screen, s32 offset) {
    screen->sprites[battle->effectStep.cursor + offset].highlight &= ~1;
    screen->sprites[battle->effectStep.cursor + offset].moving = 0;
    closeTableWindows(screen);
    battle->effectStep.vars[0] = 0;
    battle->effectStep.time = 0;
}

/* Sets up the table view's next state (effectStep.nextState): opening the
   panels, a row, leaving a row, closing, or the no-cards message */
static inline void setUpViewState(CardBattle *battle, CardScreen *screen) {
    switch (battle->effectStep.nextState) {
    case 1:
        CARDGAME_savedPanelScales[0] = screen->panels[0].scale.state;
        CARDGAME_savedPanelScales[1] = screen->panels[1].scale.state;
        screen->panels[0].scale.state = 0;
        screen->panels[1].scale.state = 0;
        battle->anim.dimAll = CARD_ANIM_UNDIM_ALL;
        battle->anim.next = CARD_ANIM_SHOW_SLOTS;
        battle->effectStep.vars[1] = CARDGAME_getTableHeight(battle, screen);
        battle->effectStep.cursor = 0;
        screen->resetPanels(screen);
        screen->openPanels(screen);
        CARDGAME_clearCardInfo(battle, screen);
        break;
    case 2:
        startTableRow(battle, screen, 0);
        break;
    case 4:
        startTableRow(battle, screen, 1);
        break;
    case 3:
        startTableRow(battle, screen, 2);
        break;
    case 8:
        leaveTableRow(battle, screen, 0);
        break;
    case 9:
        leaveTableRow(battle, screen, 12);
        break;
    case 10:
        leaveTableRow(battle, screen, 6);
        break;
    case 11:
        screen->closePanels(screen);
        closeTableWindows(screen);
        battle->anim.next = CARD_ANIM_HIDE_SLOTS;
        break;
    case 12:
        screen->openMessage(screen, 0x17, 0, 0, 1);
        break;
    case 14:
        screen->closeMessage(screen);
        screen->closePanels(screen);
        break;
    case 15:
        /* nothing to set up: it ends */
        break;
    }
}

/* Once the panels are open, shows the first row with cards in it, or the
   no-cards message */
static inline void chooseFirstRow(CardBattle *battle, CardScreen *screen) {
    if (screen->panels[0].state == 2 && battle->anim.current == CARD_ANIM_NONE) {
        if (battle->players[0].slotCount != 0) {
            battle->effectStep.nextState = 2;
            battle->effectStep.vars[3] = 0;
        } else if (battle->players[1].slotCount != 0) {
            battle->effectStep.nextState = 4;
            battle->effectStep.vars[3] = 1;
        } else if (battle->record.playCount > 0) {
            battle->effectStep.nextState = 3;
            battle->effectStep.vars[3] = 2;
        } else {
            battle->effectStep.nextState = 12;
            battle->effectStep.vars[3] = 3;
        }
    }
}

/* The player's own row: up goes to the played or the opponent's row, left
   and right move along it, triangle leaves */
static inline void browseOwnRow(CardBattle *battle, CardScreen *screen) {
    if (PAD_PRESSED(PAD_UP) && (battle->record.playCount > 0 || battle->players[1].slotCount != 0)) {
        battle->effectStep.nextState = 8;
    }
    if ((PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT))) | (PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT)))) {
        if (battle->effectStep.cursor < battle->players[0].slotCount - 1) {
            CARDGAME_moveTableHighlight(battle, screen, 0, 1);
        }
    }
    if ((PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_LEFT))) | (PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_LEFT)))) {
        if (battle->effectStep.cursor > 0) {
            CARDGAME_moveTableHighlight(battle, screen, 0, -1);
        }
    }
    CARDGAME_showTableCardInfo(battle, screen, 0);
    if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        battle->effectStep.nextState = 11;
    }
}

/* The opponent's row: down goes to the played or the player's own row, left
   and right move along it, triangle leaves */
static inline void browseOpponentRow(CardBattle *battle, CardScreen *screen) {
    if (PAD_PRESSED(PAD_DOWN) && (battle->record.playCount > 0 || battle->players[0].slotCount != 0)) {
        battle->effectStep.nextState = 10;
    }
    if ((PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT))) | (PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT)))) {
        if (battle->effectStep.cursor < battle->players[1].slotCount - 1) {
            CARDGAME_moveTableHighlight(battle, screen, 1, 1);
        }
    }
    if ((PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_LEFT))) | (PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_LEFT)))) {
        if (battle->effectStep.cursor > 0) {
            CARDGAME_moveTableHighlight(battle, screen, 1, -1);
        }
    }
    CARDGAME_showTableCardInfo(battle, screen, 1);
    if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        battle->effectStep.nextState = 11;
    }
}

/* The row of the cards played: up and down go to the opponent's or the
   player's own row, left and right move along it, triangle leaves */
static inline void browsePlayedRow(CardBattle *battle, CardScreen *screen) {
    if (PAD_PRESSED(PAD_UP)) {
        if (battle->players[1].slotCount != 0) {
            battle->effectStep.vars[1] = 1;
            battle->effectStep.nextState = 9;
        }
    } else if (PAD_PRESSED(PAD_DOWN) && battle->players[0].slotCount != 0) {
        battle->effectStep.vars[1] = 0;
        battle->effectStep.nextState = 9;
    }
    if ((PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT))) | (PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT)))) {
        if (battle->effectStep.cursor < battle->record.playCount - 1) {
            CARDGAME_moveTableHighlight(battle, screen, 2, 1);
        }
    }
    if ((PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_LEFT))) | (PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_LEFT)))) {
        if (battle->effectStep.cursor > 0) {
            CARDGAME_moveTableHighlight(battle, screen, 2, -1);
        }
    }
    if (PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        battle->effectStep.nextState = 11;
    }
    CARDGAME_showTableCardInfo(battle, screen, 2);
}

/* 11 frames after leaving the player's own row, goes up to the played row
   (the cursor halved) or the opponent's row */
static inline void leaveOwnRow(CardBattle *battle, CardScreen *screen) {
    battle->effectStep.time++;
    if (battle->effectStep.time >= 11) {
        if (battle->record.playCount > 0) {
            battle->effectStep.nextState = 3;
            battle->effectStep.cursor /= 2;
            if (battle->effectStep.cursor > battle->record.playCount - 1) {
                battle->effectStep.cursor = battle->record.playCount - 1;
            }
        } else if (battle->players[1].slotCount != 0) {
            battle->effectStep.nextState = 4;
            if (battle->effectStep.cursor > battle->players[1].slotCount - 1) {
                battle->effectStep.cursor = battle->players[1].slotCount - 1;
            }
        }
    }
}

/* 11 frames after leaving the played row, goes to the opponent's
   (effectStep.vars[1]) or the player's own row, the cursor doubled */
static inline void leavePlayedRow(CardBattle *battle, CardScreen *screen) {
    battle->effectStep.time++;
    if (battle->effectStep.time >= 11) {
        battle->effectStep.cursor = battle->effectStep.cursor * 2 + 1;
        if (battle->effectStep.vars[1] != 0) {
            battle->effectStep.nextState = 4;
            if (battle->effectStep.cursor > battle->players[1].slotCount - 1) {
                battle->effectStep.cursor = battle->players[1].slotCount - 1;
            }
        } else {
            battle->effectStep.nextState = 2;
            if (battle->effectStep.cursor > battle->players[0].slotCount - 1) {
                battle->effectStep.cursor = battle->players[0].slotCount - 1;
            }
        }
    }
}

/* 11 frames after leaving the opponent's row, goes down to the played row
   (the cursor halved) or the player's own row */
static inline void leaveOpponentRow(CardBattle *battle, CardScreen *screen) {
    battle->effectStep.time++;
    if (battle->effectStep.time >= 11) {
        if (battle->record.playCount > 0) {
            battle->effectStep.nextState = 3;
            battle->effectStep.cursor /= 2;
            if (battle->effectStep.cursor > battle->record.playCount - 1) {
                battle->effectStep.cursor = battle->record.playCount - 1;
            }
        } else if (battle->players[0].slotCount != 0) {
            battle->effectStep.nextState = 2;
            if (battle->effectStep.cursor > battle->players[0].slotCount - 1) {
                battle->effectStep.cursor = battle->players[0].slotCount - 1;
            }
        }
    }
}

/* Lets the player look over the cards out on the table: the two players' rows
   and the row of the cards played (up and down change rows, left and right
   move along one, triangle leaves), each state set up by setUpViewState; 1
   once it is over */
s32 CARDGAME_viewTable(CardBattle *battle, CardScreen *screen) {
    s32 done = 0;

    if (battle->effectStep.nextState != 0) {
        setUpViewState(battle, screen);
        battle->effectStep.state = battle->effectStep.nextState;
        battle->effectStep.nextState = 0;
    }
    switch (battle->effectStep.state) {
    case 1:
        chooseFirstRow(battle, screen);
        break;
    case 2:
    case 3:
    case 4:
        battle->effectStep.vars[0] = CARDGAME_openStepWindows(battle, screen, battle->effectStep.vars[3], 0, battle->effectStep.time, battle->effectStep.vars[0]);
        CARDGAME_showCardInfo(battle, screen, CARDGAME_rowSpriteOffsets[battle->effectStep.vars[3]]);
        battle->effectStep.time++;
        if (battle->effectStep.time >= 11) {
            CARDGAME_moveTableHighlight(battle, screen, battle->effectStep.vars[3], 0);
            battle->effectStep.nextState = CARDGAME_rowSteps[battle->effectStep.vars[3]];
        }
        break;
    case 5:
        browseOwnRow(battle, screen);
        break;
    case 6:
        browseOpponentRow(battle, screen);
        break;
    case 7:
        browsePlayedRow(battle, screen);
        break;
    case 8:
        leaveOwnRow(battle, screen);
        break;
    case 9:
        leavePlayedRow(battle, screen);
        break;
    case 10:
        leaveOpponentRow(battle, screen);
        break;
    case 11:
        if (screen->panels[0].state == 0 && battle->anim.current == CARD_ANIM_NONE) {
            battle->effectStep.nextState = 15;
        }
        break;
    case 12:
        if (screen->message.state == 2) {
            battle->effectStep.nextState = 13;
        }
        break;
    case 13:
        if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
            battle->effectStep.nextState = 14;
        }
        break;
    case 14:
        if (screen->message.state == 0) {
            battle->effectStep.nextState = 15;
        }
        break;
    case 15:
        screen->panels[0].scale.state = CARDGAME_savedPanelScales[0];
        screen->panels[1].scale.state = CARDGAME_savedPanelScales[1];
        done = 1;
        break;
    }
    return done;
}

/* Starts CARDGAME_pickTableCard, which triangle backs out of when cancelable
   (effectStep.vars[4]) */
void CARDGAME_startPickTableCard(CardBattle *battle, CardScreen *screen, s32 cancelable) {
    battle->effectStep.vars[4] = cancelable;
    battle->effectStep.nextState = 1;
}

/* CARDGAME_getTableHeight, for CARDGAME_pickTableCard */
s32 CARDGAME_getPickTableHeight(CardBattle *battle, CardScreen *screen) {
    return CARDGAME_getTableHeight(battle, screen);
}

/* CARDGAME_moveTableHighlight, for CARDGAME_pickTableCard */
void CARDGAME_movePickHighlight(CardBattle *battle, CardScreen *screen, s32 kind, s32 delta) {
    CARDGAME_moveTableHighlight(battle, screen, kind, delta);
}

/* Sets up the table pick's next state (effectStep.nextState): dimming the
   cards that can't be picked, a row, leaving a row, the pick, or closing */
static inline void setUpPickState(CardBattle *battle, CardScreen *screen) {
    s32 i;
    /* the match depends on a second loop variable: GCC 2.8.1 gives the first
       two loops other registers than it gives i */
    s32 j;

    switch (battle->effectStep.nextState) {
    case 1:
        if (battle->effectStep.vars[4] != 0) {
            for (j = 0; j < 3; j++) {
                battle->anim.dimmed[j + 12] = 1;
            }
            for (j = 0; j < 12; j++) {
                if (battle->effectStep.eligible[j] != 0) {
                    battle->anim.dimmed[j] = 0;
                } else {
                    battle->anim.dimmed[j] = 1;
                }
            }
            battle->anim.next = CARD_ANIM_SHOW_SLOTS;
            battle->effectStep.cursor = 0;
            screen->resetPanels(screen);
            screen->openPanels(screen);
            screen->addSprite(screen, 15, 0xE500, 0x6100);
            screen->setSpriteCard(screen, 15, battle->record.plays[battle->record.playCount].card);
            screen->sprites[15].scaleX = 0;
            screen->scaleSprite(screen, 15, 8, 0x1000, 0x1000);
        } else {
            for (i = 0; i < 3; i++) {
                screen->sprites[i + 12].dimmed = 1;
            }
            for (i = 0; i < 12; i++) {
                if (battle->effectStep.eligible[i] != 0) {
                    screen->sprites[i].dimmed = 0;
                } else {
                    screen->sprites[i].dimmed = 1;
                }
            }
        }
        battle->effectStep.vars[1] = CARDGAME_getPickTableHeight(battle, screen);
        CARDGAME_clearCardInfo(battle, screen);
        break;
    case 2:
        startTableRow(battle, screen, 0);
        break;
    case 3:
        startTableRow(battle, screen, 1);
        break;
    case 7:
        leaveTableRow(battle, screen, 0);
        break;
    case 8:
        leaveTableRow(battle, screen, 6);
        break;
    case 6:
        closeTableWindows(screen);
        screen->startBlink(screen, battle->effectStep.choice);
        SOUND.playSound(SOUND_MENU_CONFIRM);
        break;
    case 9:
        screen->closePanels(screen);
        closeTableWindows(screen);
        battle->anim.next = CARD_ANIM_HIDE_SLOTS;
        screen->scaleSprite(screen, 15, 8, 0, 0x1000);
        break;
    case 10:
        screen->closePanels(screen);
        battle->anim.next = CARD_ANIM_HIDE_SLOTS;
        screen->scaleSprite(screen, 15, 8, 0, 0x1000);
        break;
    case 14:
        /* nothing to set up */
        break;
    }
}

/* Once the panels are open (or right away without them), shows the first
   row the pick allows that has cards in it, or the no-cards message */
static inline void chooseFirstPickRow(CardBattle *battle, CardScreen *screen) {
    if (battle->effectStep.vars[4] == 0 || (screen->panels[0].state == 2 && battle->anim.current == CARD_ANIM_NONE)) {
        if (battle->effectStep.flags & 1) {
            if (battle->players[0].slotCount != 0) {
                battle->effectStep.nextState = 2;
                battle->effectStep.vars[3] = 0;
            } else {
                battle->effectStep.flags &= ~1;
            }
        }
        if (battle->effectStep.flags & 2) {
            if (battle->players[1].slotCount != 0) {
                battle->effectStep.nextState = 3;
                battle->effectStep.vars[3] = 1;
            } else {
                battle->effectStep.flags &= ~2;
            }
        }
        if (battle->effectStep.flags == 0) {
            battle->effectStep.nextState = 12;
            battle->effectStep.vars[3] = 3;
        }
    }
}

/* The player's own row: up goes to the opponent's row when allowed, left and
   right move along it, triangle backs out (with effectStep.vars[4]), cross
   picks an eligible card */
static inline void pickInOwnRow(CardBattle *battle, CardScreen *screen) {
    if ((battle->effectStep.flags & 2) && PAD_PRESSED(PAD_UP) && battle->players[1].slotCount != 0) {
        battle->effectStep.nextState = 7;
    }
    if ((PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT))) | (PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT)))) {
        if (battle->effectStep.cursor < battle->players[0].slotCount - 1) {
            CARDGAME_moveTableHighlight(battle, screen, 0, 1);
        }
    }
    if ((PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_LEFT))) | (PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_LEFT)))) {
        if (battle->effectStep.cursor > 0) {
            CARDGAME_moveTableHighlight(battle, screen, 0, -1);
        }
    }
    CARDGAME_showTableCardInfo(battle, screen, 0);
    if (battle->effectStep.vars[4] != 0 && PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        battle->effectStep.nextState = 9;
    }
    if (PAD_PRESSED(PAD_CROSS) && battle->effectStep.eligible[battle->effectStep.cursor] != 0) {
        battle->effectStep.nextState = 6;
        battle->effectStep.choice = battle->effectStep.cursor;
    }
}

/* The opponent's row: down goes to the player's own row when allowed, left
   and right move along it, triangle backs out (with effectStep.vars[4]),
   else cross picks an eligible card */
static inline void pickInOpponentRow(CardBattle *battle, CardScreen *screen) {
    if ((battle->effectStep.flags & 1) && PAD_PRESSED(PAD_DOWN) && battle->players[0].slotCount != 0) {
        battle->effectStep.nextState = 8;
    }
    if ((PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT))) | (PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_RIGHT)))) {
        if (battle->effectStep.cursor < battle->players[1].slotCount - 1) {
            CARDGAME_moveTableHighlight(battle, screen, 1, 1);
        }
    }
    if ((PAD.getPressed(0) & (1 << PAD.getButtonBit(0, PAD_LEFT))) | (PAD.getRepeated(0) & (1 << PAD.getButtonBit(0, PAD_LEFT)))) {
        if (battle->effectStep.cursor > 0) {
            CARDGAME_moveTableHighlight(battle, screen, 1, -1);
        }
    }
    CARDGAME_showTableCardInfo(battle, screen, 1);
    if (battle->effectStep.vars[4] != 0 && PAD_PRESSED(PAD_TRIANGLE)) {
        SOUND.playSound(SOUND_MENU_CANCEL);
        battle->effectStep.nextState = 9;
    } else if (PAD_PRESSED(PAD_CROSS) && battle->effectStep.eligible[battle->effectStep.cursor + 6] != 0) {
        battle->effectStep.nextState = 6;
        battle->effectStep.choice = battle->effectStep.cursor + 6;
    }
}

/* Lets the player pick a card out on the table (effectStep.flags bit 0: in their own
   row, bit 1: in the opponent's): 2 once one is picked (its sprite in
   effectStep.choice), 1 if there was none or the player backed out; each
   state is set up by setUpPickState */
s32 CARDGAME_pickTableCard(CardBattle *battle, CardScreen *screen) {
    s32 result = 0;
    s32 i;

    if (battle->effectStep.nextState != 0) {
        setUpPickState(battle, screen);
        battle->effectStep.state = battle->effectStep.nextState;
        battle->effectStep.nextState = 0;
    }
    switch (battle->effectStep.state) {
    case 1:
        chooseFirstPickRow(battle, screen);
        break;
    case 2:
    case 3:
        battle->effectStep.vars[0] = CARDGAME_openStepWindows(battle, screen, battle->effectStep.vars[3], 0, battle->effectStep.time, battle->effectStep.vars[0]);
        CARDGAME_showCardInfo(battle, screen, CARDGAME_rowSpriteOffsets[battle->effectStep.vars[3]]);
        battle->effectStep.time++;
        if (battle->effectStep.time >= 11) {
            switch (battle->effectStep.vars[3]) {
            case 0:
                CARDGAME_movePickHighlight(battle, screen, 0, 0);
                battle->effectStep.nextState = 4;
                break;
            case 1:
                CARDGAME_movePickHighlight(battle, screen, 1, 0);
                battle->effectStep.nextState = 5;
                break;
            }
        }
        break;
    case 4:
        pickInOwnRow(battle, screen);
        break;
    case 5:
        pickInOpponentRow(battle, screen);
        break;
    case 6:
        if (screen->sprites[battle->effectStep.choice].state == 1) {
            if (battle->effectStep.vars[4] == 0) {
                for (i = 0; i < 3; i++) {
                    screen->sprites[i + 12].dimmed = 0;
                }
                for (i = 0; i < 12; i++) {
                    screen->sprites[i].dimmed = 0;
                }
            }
            battle->effectStep.nextState = 14;
        }
        break;
    case 7:
        battle->effectStep.time++;
        if (battle->effectStep.time >= 11) {
            battle->effectStep.nextState = 3;
            if (battle->effectStep.cursor > battle->players[1].slotCount - 1) {
                battle->effectStep.cursor = battle->players[1].slotCount - 1;
            }
        }
        break;
    case 8:
        battle->effectStep.time++;
        if (battle->effectStep.time >= 11) {
            battle->effectStep.nextState = 2;
            if (battle->effectStep.cursor > battle->players[0].slotCount - 1) {
                battle->effectStep.cursor = battle->players[0].slotCount - 1;
            }
        }
        break;
    case 11:
        battle->effectStep.nextState = 14;
        break;
    case 9:
    case 10:
        if (screen->panels[0].state == 0 && battle->anim.current == CARD_ANIM_NONE) {
            battle->effectStep.nextState = 13;
        }
        break;
    case 12:
        if (PAD_PRESSED(PAD_TRIANGLE)) {
            SOUND.playSound(SOUND_MENU_CANCEL);
            battle->effectStep.nextState = 10;
        }
        break;
    case 13:
        result = 1;
        break;
    case 14:
        for (i = 0; i < 15; i++) {
            battle->effectStep.marked[i] = 0;
        }
        result = 2;
        battle->effectStep.marked[battle->effectStep.choice] = 1;
        break;
    }
    return result;
}
