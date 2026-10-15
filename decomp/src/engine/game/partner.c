#include "engine/game.h"

/* The index of Digimon `id` among a partner's entries, or -1 */
s32 findPartnerEntry(s32 partner, s32 id) {
    s32 i;

    for (i = 0; i < PARTNER_ENTRY_COUNT; i++) {
        if (GAME.partners[partner].info.entries[i].id < FIRST_ENTRY_ID) {
            continue;
        }
        if (GAME.partners[partner].info.entries[i].id == id) {
            return i;
        }
    }
    return -1;
}

/* Copies the ids of the Digimon a partner takes to battle; returns how many there are */
s32 getPartnerSlots(s32 partner, s16 *out) {
    s32 i;
    s32 n;
    s32 index;

    for (i = 0, n = 0; i < PARTNER_SLOT_COUNT; i++) {
        if (GAME.partners[partner].info.slots[i] >= FIRST_ENTRY_ID) {
            index = findPartnerEntry(partner, GAME.partners[partner].info.slots[i]);
            if (index >= 0 && GAME.partners[partner].info.entries[index].id >= FIRST_ENTRY_ID) {
                out[n] = GAME.partners[partner].info.entries[index].id;
                n++;
            }
        }
    }
    for (i = n; i < PARTNER_SLOT_COUNT; i++) {
        out[i] = -1;
    }
    return n;
}

/* Picks the Digimon a partner takes to battle (-1 for ids it does not have) */
void setPartnerSlots(s32 partner, s16 *ids) {
    s32 i;
    s32 index;

    for (i = 0; i < PARTNER_SLOT_COUNT; i++) {
        index = findPartnerEntry(partner, ids[i]);
        if (index >= 0) {
            GAME.partners[partner].info.slots[i] = GAME.partners[partner].info.entries[index].id;
        } else {
            GAME.partners[partner].info.slots[i] = -1;
        }
    }
}

/* Copies the ids of a partner's Digimon, zero-filled; returns how many there are */
s32 listPartnerEntries(s32 partner, u16 *out) {
    s32 i;
    s32 n;
    s32 count;

    for (n = i = 0; i < PARTNER_ENTRY_COUNT; i++) {
        if (GAME.partners[partner].info.entries[i].id >= FIRST_ENTRY_ID) {
            out[n] = GAME.partners[partner].info.entries[i].id;
            n++;
        }
    }
    count = n;
    for (; n < PARTNER_ENTRY_COUNT; n++) {
        out[n] = 0;
    }
    return count;
}

/* Gives a partner Digimon `id` at level 1; 0 if it has it already or has no room */
s32 addPartnerEntry(s32 partner, s32 id) {
    s32 i;
    s32 index;

    if (findPartnerEntry(partner, id) != -1) {
        return 0;
    }
    for (i = 0, index = -1; i < PARTNER_ENTRY_COUNT; i++) {
        if (GAME.partners[partner].info.entries[i].id == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return 0;
    }
    GET_DIGIMON(id); /* its result goes unused */
    GAME.partners[partner].info.entries[index].id = id;
    GAME.partners[partner].info.entries[index].level = 1;
    return 1;
}

/* Copies a partner's Digimon `id`; returns its index, or -1 */
s32 getPartnerEntry(s32 partner, s32 id, PartnerEntry *out) {
    s32 i = findPartnerEntry(partner, id);

    if (i != -1) {
        *out = GAME.partners[partner].info.entries[i];
    }
    return i;
}

/* Overwrites a partner's Digimon `id`; returns its index, or -1 */
s32 setPartnerEntry(s32 partner, s32 id, PartnerEntry *in) {
    s32 i = findPartnerEntry(partner, id);

    if (i != -1) {
        GAME.partners[partner].info.entries[i] = *in;
    }
    return i;
}

/* A partner's name, stats and Digimon */
PartnerStats *getPartnerStats(s32 partner) {
    return &GAME.partners[partner].info;
}
