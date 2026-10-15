#include "engine/game.h"
#include "field/field_map.h"

/* The new game's player name, deck names, starter deck and partners */
void initNewGameData(void) {
    TextTools cls;
    s32 i;
    s32 j;
    s16 *dst;
    u16 *src;
    DigimonData *e;

    initTextTools(&cls);
    strcpy(GAME.name, cls.getString(FILE_CACHE.load(TEXT_FILE(TEXT_NAME_ENTRY)), 0xB));
    GAME.party[0] = -1;
    GAME.party[1] = -1;
    GAME.party[2] = -1;
    for (j = 0; j < DECK_COUNT; j++) {
        strcpy(GAME.decks[j].name, cls.getString(FILE_CACHE.load(TEXT_FILE(TEXT_CARD_SHOP)), j + 0x16));
    }
    GAME.funcs.giveStarterDeck();
    for (i = 0; i < PARTNER_COUNT; i++) {
        e = &DIGIMON_DATA[i];
        strcpy(GAME.partners[i].info.name, cls.getString(FILE_CACHE.load(TEXT_FILE(TEXT_DIGIMON_NAMES)), e->nameId));
        GAME.partners[i].info.stats[STAT_LEVEL] = 1;
        GAME.partners[i].info.stats[STAT_HP] = GAME.partners[i].info.stats[STAT_MAX_HP] = e->hp;
        GAME.partners[i].info.stats[STAT_MP] = GAME.partners[i].info.stats[STAT_MAX_MP] = e->mp;
        /* the battle stats, then the resistances */
        dst = &GAME.partners[i].info.stats[STAT_STRENGTH];
        src = e->battleStats;
        for (j = 0; j < 6; j++) {
            *dst++ = *src++;
        }
        src = e->resistances;
        for (j = 0; j < 7; j++) {
            *dst++ = *src++;
        }
    }
}

/* The partner index of party member `index`, or -1 */
s32 getPartyMember(u32 index) {
    if (index >= PARTY_SIZE) {
        return -1;
    }
    return GAME.party[index];
}

/* Gives the party STARTER_PARTIES[set], unlocking its partners */
void setParty(s32 set) {
    s32 i;
    u8 partner;

    for (i = 0; i < PARTY_SIZE; i++) {
        partner = STARTER_PARTIES[set][i];
        GAME.party[i] = partner;
        GAME.partners[partner].unlocked = partner + 3;
    }
    GAME.partySet = set;
}

/* Gives `count` copies of a card (up to CARD_COPIES_MAX) and marks it seen */
void addCards(s32 card, s32 count) {
    GAME.cardsSeen[card] = 1;
    GAME.cards[card] += count;
    if (GAME.cards[card] > CARD_COPIES_MAX) {
        GAME.cards[card] = CARD_COPIES_MAX;
    }
}

/* Gives the starter deck's cards and puts them in every deck */
void giveStarterDeck(void) {
    s32 i;
    s32 j;

    for (i = 0; i < DECK_SIZE; i++) {
        addCards(STARTER_DECK[i], 1);
    }
    for (j = 0; j < DECK_COUNT; j++) {
        for (i = 0; i < DECK_SIZE; i++) {
            GAME.decks[j].cards[i] = STARTER_DECK[i];
        }
    }
}

/* setParty's partners, by party set */
u8 STARTER_PARTIES[][3] = {
    { 0x00, 0x06, 0x07 }, { 0x02, 0x03, 0x06 }, { 0x01, 0x05, 0x07 },
};

/* The cards of giveStarterDeck's deck */
s32 STARTER_DECK[DECK_SIZE] = {
    24, 24, 24, 45, 45, 50, 59, 60, 95, 99,
    100, 100, 102, 122, 138, 141, 141, 143, 145, 181,
    184, 185, 185, 188, 221, 224, 228, 230, 230, 230,
    267, 268, 268, 270, 274, 303, 313, 314, 314, 314,
};
