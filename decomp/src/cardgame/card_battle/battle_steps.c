/* CARDGAME's battle steps that wait on the screen: a message, the panels
   closing and opening, the slots' values counted, and the conditions of a
   play. */

#include "cardgame/card_battle.h"

/* Opens message text for a step */
void CARDGAME_startMessage(CardBattle *battle, CardScreen *screen, s32 text) {
    battle->effectStep.time = 0;
    screen->openMessage(screen, text, 0, 0, 1);
    battle->effectStep.state = 1;
}

/* Waits for the message window to open, for cross or triangle, then for the window to close; 1 once it has */
s32 CARDGAME_waitMessage(CardBattle *battle, CardScreen *screen) {
    s32 done = 0;

    switch (battle->effectStep.state) {
    case 1:
        if (screen->message.state == 2) {
            battle->effectStep.state = 2;
        }
        break;
    case 2:
        if (PAD_PRESSED(PAD_CROSS) || PAD_PRESSED(PAD_TRIANGLE)) {
            battle->effectStep.state = 3;
            screen->closeMessage(screen);
        }
        break;
    case 3:
        if (screen->message.state == 0) {
            battle->effectStep.state = 4;
        }
        break;
    case 4:
        done = 1;
        break;
    }
    return done;
}

/* When the last card was the player's (or force): takes its play off
   for a while and closes the panels */
void CARDGAME_startClosePanels(CardBattle *battle, CardScreen *screen, s32 force) {
    if (battle->record.plays[battle->record.playCount - 1].side == 0 || force) {
        battle->record.playCount--;
        screen->closePanels(screen);
        battle->anim.next = CARD_ANIM_HIDE_SLOTS;
        battle->effectStep.state = 1;
    } else {
        battle->effectStep.state = 2;
    }
}

/* Waits for the panels to close and puts the play back; 1 once done */
s32 CARDGAME_stepClosePanels(CardBattle *battle, CardScreen *screen) {
    s32 done = 0;

    switch (battle->effectStep.state) {
    case 1:
        if (screen->panels[0].state == 0 && battle->anim.current == CARD_ANIM_NONE) {
            battle->effectStep.state = 2;
            battle->record.playCount++;
        }
        break;
    case 2:
        done = 1;
        break;
    }
    return done;
}

/* When the last card was the player's (or force): takes its play off
   for a while and opens the panels */
void CARDGAME_startOpenPanels(CardBattle *battle, CardScreen *screen, s32 force) {
    if (battle->record.plays[battle->record.playCount - 1].side == 0 || force) {
        screen->openPanels(screen);
        battle->anim.dimAll = CARD_ANIM_UNDIM_ALL;
        battle->anim.next = CARD_ANIM_SHOW_SLOTS;
        battle->effectStep.state = 1;
        battle->record.playCount--;
    } else {
        battle->effectStep.state = 2;
    }
}

/* Waits for the panels to open and puts the play back; 1 once done */
s32 CARDGAME_stepOpenPanels(CardBattle *battle, CardScreen *screen) {
    s32 done = 0;

    switch (battle->effectStep.state) {
    case 1:
        if (battle->anim.current == CARD_ANIM_NONE && screen->panels[0].state == 2) {
            battle->effectStep.state = 2;
            battle->record.playCount++;
        }
        break;
    case 2:
        done = 1;
        break;
    }
    return done;
}

/* Steps each slot's shown values (its sprite's ap and hp) one unit toward the slot's ap and hp, with a sound; 1 once all are there, 2 if a slot's hp reached 0 (marked in effectStep.marked) */
s32 CARDGAME_countSlotValues(CardBattle *battle, CardScreen *screen) {
    s32 done = 0;
    s32 changed = 0;
    s32 side;
    s32 i;
    CardSprite *sprites;

    switch (battle->effectStep.state) {
    case 1:
    default:
        for (i = 0; i < 12; i++) {
            battle->effectStep.marked[i] = 0;
        }
        battle->effectStep.state = 2;
        battle->effectStep.vars[4] = 0;
        break;
    case 2:
        done = 1;
        for (side = 0; side < 2; side++) {
            if (side == 0) {
                sprites = &screen->sprites[0];
            } else {
                sprites = &screen->sprites[6];
            }
            for (i = 0; i < battle->players[side].slotCount; i++) {
                if (sprites[i].ap != battle->players[side].slots[i].ap) {
                    if (sprites[i].ap > battle->players[side].slots[i].ap) {
                        sprites[i].ap--;
                    } else {
                        sprites[i].ap++;
                    }
                    done = 0;
                    changed = 1;
                }
                if (sprites[i].hp != battle->players[side].slots[i].hp) {
                    if (sprites[i].hp > battle->players[side].slots[i].hp) {
                        sprites[i].hp--;
                    } else {
                        sprites[i].hp++;
                    }
                    done = 0;
                    if (battle->players[side].slots[i].hp == 0) {
                        battle->effectStep.marked[i + side * 6] = 1;
                        battle->effectStep.vars[4] = 1;
                    }
                    changed = 1;
                }
            }
        }
        if (done && battle->effectStep.vars[4]) {
            done = 2;
        }
        if (changed) {
            SOUND.playSound(SOUND_COUNT);
        }
        break;
    }
    return done;
}

/* Whether the condition kind (1-11) of the card being played holds for its side: 1 if it does */
s32 CARDGAME_checkPlayCondition(CardBattle *battle, CardScreen *screen, s32 kind) {
    CardDrawer drawer;
    CardDrawer drawer2;
    CardDrawer drawer3;
    CardDrawer drawer4;
    s32 ok = 0;
    s32 side = battle->record.plays[battle->record.playCount - 1].side;
    s32 other = side ^ 1;
    s32 i;
    s32 slot;
    s32 j;
    CardSlot *p;
    s32 n;
    s32 k;
    s32 valid;

    switch (kind) {
    case 1:
        if (battle->sides[other].pile.handCount == 0) {
            ok = 1;
        }
        break;
    case 2:
        initCardDrawer(&drawer);
        ok = 1;
        for (i = 0; i < battle->sides[other].pile.handCount; i++) {
            drawer.setCard(battle->cards[battle->sides[other].pile.hand[i]] + 1);
            if (drawer.card->color == 6 && drawer.card->kind == 0x10) {
                ok = 0;
                break;
            }
        }
        break;
    case 3:
        initCardDrawer(&drawer2);
        ok = 1;
        for (i = 0; i < battle->sides[other].pile.handCount; i++) {
            drawer2.setCard(battle->cards[battle->sides[other].pile.hand[i]] + 1);
            if (drawer2.card->color != 5) {
                ok = 0;
                break;
            }
        }
        break;
    case 4:
        if (battle->sides[side].pile.discardCount == 0) {
            ok = 1;
        }
        break;
    case 5:
        if (battle->sides[side].pile.deckCount == 0) {
            ok = 1;
        }
        break;
    case 6:
        if (battle->sides[other].pile.deckCount == 0) {
            ok = 1;
        }
        break;
    case 7:
        initCardDrawer(&drawer3);
        ok = 1;
        for (i = battle->sides[side].pile.deckTop; i < 40; i++) {
            drawer3.setCard(battle->cards[battle->sides[side].pile.deck[i]] + 1);
            if (drawer3.card->kind == 0x10) {
                ok = 0;
                break;
            }
        }
        break;
    case 8:
        initCardDrawer(&drawer4);
        ok = 1;
        for (i = battle->sides[side].pile.deckTop; i < 40; i++) {
            drawer4.setCard(battle->cards[battle->sides[side].pile.deck[i]] + 1);
            if (drawer4.card->kind != 0x10) {
                ok = 0;
                break;
            }
        }
        break;
    case 9:
        ok = 1;
        n = battle->record.playCount - 1;
        for (slot = 0; slot < 12; slot++) {
            battle->effectStep.marked[slot] = 0;
            if (slot < 6) {
                if (slot >= battle->players[0].slotCount) {
                    continue;
                }
                p = &battle->players[0].slots[slot];
            } else {
                if (slot - 6 >= battle->players[1].slotCount) {
                    continue;
                }
                p = &battle->players[1].slots[slot - 6];
            }
            if (p->order == battle->record.plays[n].target) {
                ok = 0;
                break;
            }
        }
        break;
    case 10:
        ok = 1;
        for (i = 0; i < 12 && ok == 1; i++) {
            k = i - 6;
            if (i < 6) {
                valid = i < battle->players[0].slotCount;
            } else {
                valid = k < battle->players[1].slotCount;
            }
            if (valid) {
                switch (battle->record.plays[battle->record.playCount - 1].targetKind) {
                case 1:
                    if (side == 0) {
                        if (i < 6) {
                            ok = 0;
                        }
                    } else {
                        if (i >= 6) {
                            ok = 0;
                        }
                    }
                    break;
                case 2:
                    if (side == 0) {
                        if (i >= 6) {
                            ok = 0;
                        }
                    } else {
                        if (i < 6) {
                            ok = 0;
                        }
                    }
                    break;
                case 3:
                    ok = 0;
                    break;
                case 4:
                    if (screen->getCardColor(screen, i < 6 ? battle->players[0].slots[i].card : battle->players[1].slots[i - 6].card) != 1) {
                        ok = 0;
                    }
                    break;
                case 5:
                    if (screen->getCardColor(screen, i < 6 ? battle->players[0].slots[i].card : battle->players[1].slots[i - 6].card) != 2) {
                        ok = 0;
                    }
                    break;
                case 6:
                    if (screen->getCardColor(screen, i < 6 ? battle->players[0].slots[i].card : battle->players[1].slots[i - 6].card) == 3) {
                        ok = 0;
                    }
                    break;
                case 7:
                    if (screen->getCardColor(screen, i < 6 ? battle->players[0].slots[i].card : battle->players[1].slots[i - 6].card) != 4) {
                        ok = 0;
                    }
                    break;
                case 8:
                    if (screen->getCardColor(screen, i < 6 ? battle->players[0].slots[i].card : battle->players[1].slots[i - 6].card) == 6) {
                        ok = 0;
                    }
                    break;
                }
            }
        }
        break;
    case 11:
        ok = 1;
        for (j = 0; j < battle->sides[side].pile.handCount; j++) {
            if (battle->record.plays[battle->record.playCount - 1].target == battle->sides[side].pile.hand[j]) {
                ok = 0;
                break;
            }
        }
        break;
    }
    return ok;
}
