#ifndef DW3_GAME_STATE_H
#define DW3_GAME_STATE_H

/* The game state: modes, party, items, cards, flags (game/) */

#include "common.h"
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include "engine/gfx.h" /* Vec2 */

union PartnerTotals;

/* The game state's methods (GAME.funcs) */
typedef struct GameFuncs {
    /* 0x00 */ void (*newGame)();
    /* 0x04 */ void (*commitMode)();
    /* 0x08 */ s32 (*getMode)(void);
    /* 0x0C */ s32 (*getModeArg)();
    /* 0x10 */ void (*requestMode)(s32 mode, s32 arg);
    /* 0x14 */ s32 (*isModeChangePending)();
    /* 0x18 */ s32 (*getPrevMode)();
    /* 0x1C */ s32 (*getPartyMember)(u32 index);
    /* 0x20 */ void (*setParty)();
    /* 0x24 */ void (*addCards)(s32 card, s32 count);
    /* 0x28 */ void (*giveStarterDeck)(void);
    /* 0x2C */ s32 (*getPartyPartner)();
    /* 0x30 */ void (*setStat)(s32 partner, u32 stat, s16 value);
    /* 0x34 */ void (*addStat)();
    /* 0x38 */ void (*computeStats)(s32 partner, union PartnerTotals *out);
    /* 0x3C */ s32 (*getPartnerSlots)(); /* (partner, s16 *out): the count */
    /* 0x40 */ void (*setPartnerSlots)();
    /* 0x44 */ s32 (*listPartnerEntries)(); /* (partner, s16 *out): the count */
    /* 0x48 */ s32 (*addPartnerEntry)(); /* (partner, id): 0 if it has it or has no room */
    /* 0x4C */ s32 (*getPartnerEntry)(); /* -1 if it hasn't the Digimon */
    /* 0x50 */ s32 (*setPartnerEntry)();
    /* 0x54 */ struct PartnerStats *(*getPartnerStats)(s32 partner);
    /* 0x58 */ void (*resetPlayTime)();
    /* 0x5C */ void (*updatePlayTime)();
} GameFuncs;

/*
 * FLAGS_00, the first flag bitset, followed by the event functions: FIELDSTG
 * and the stages call them through here (+0xC applies an action, +0x10 checks
 * a condition).
 */
typedef struct GameFlags {
    /* 0x00 */ u8 bits[4];
    /* 0x04 */ s32 pendingFlag10; /* PENDING_FLAG_10 */
    /* 0x08 */ s32 (*checkConditions)(u16 *list);
    /* 0x0C */ void (*applyAction)(s32 code, s32 value);
    /* 0x10 */ s32 (*checkCondition)(u16 code, u16 value);
    /* 0x14 */ void (*applyActions)(u16 *list);
    /* 0x18 */ void (*updateModeFlags)(void);
} GameFlags;

/*
 * The codes of the (code, value) pairs that FLAGS_00 checks as conditions
 * and applies as actions (events.c's checkCondition and applyAction): the
 * talks' and characters' lists, which end with CODES_END, and a StageSlot's
 * two conditions, CODES_END for none. code >> 8 & 0xFE is the kind, code &
 * 0x1FF the flag, value or item. A condition holds when the flag is value,
 * or when the test is (1) or isn't (0) true; an action sets the flag to
 * value. The kinds the stages use and these don't name (0x7A, 0x7C and
 * 0x94, actions that change the mode) stay numbers.
 */
#define FLAG(group, id) ((group) << 8 | (id)) /* group 0x00-0x40: FLAGS_00, GAME.flags02-flags40 */
#define PROGRESS(n) (0x6000 | (n)) /* GAME.progress is n */
#define SPECIAL(id) (0x7000 | (id)) /* SPECIAL_CONDITIONS entry id */
#define PARTY_STAT(id) (0x7200 | (id)) /* checkPartyStat */
#define EVENT_BATTLE(id) (0x7400 | (id)) /* action: FIELDSTG_battleFuncs.startEventBattle */
#define CARD_BATTLE(opponent, kind) ((0x7600 + (kind) * 0x200) | (opponent)) /* action: startCardBattle */
#define WARP_ARG(id) (0x7E00 | (id)) /* checkWarpArg */
/* an item, in the bag or equipped; kind (bits 9-11) isn't read, and the
   scripts set it after the item's kind loosely */
#define ITEM(kind, id) (0x8000 | (kind) << 9 | (id))
#define START_EVENT(index) (0x9000 | (index)) /* action: FIELDSTG_startListedEvent */
#define CARD(id) (0x9200 | (id)) /* a card: the player has one, or gets or loses one */
#define CODES_END 0xFFFF

/* Digimon definition (DIGIMON_DATA, 52 of them; the first 8 are the partners) */
typedef struct DigimonData {
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 battleStats[6];
    /* 0x0E */ u16 resistances[7];
    /* 0x1C */ u16 skills[7]; /* [1]-[6] are learnt at skillLevels */
    /* 0x2A */ u16 pairTech; /* its pair technique, with pairPartner */
    /* 0x2C */ u8 statusResists[5]; /* against poison, paralysis, confusion, sleep and knock-outs */
    /* 0x31 */ u8 skillLevels[6];
    /* 0x37 */ u8 knownLevels[5]; /* the levels that mark an entry's skills[0]-[4] known */
    /* 0x3C */ u8 expLevel; /* the level after which its exp grows faster */
    /* 0x3D */ u8 pairPartner; /* the Digimon (from 1) it does pairTech with, 0 for none */
    /* 0x3E */ u8 expRate; /* a partner's exp per level, in tenths */
    /* 0x3F */ u8 hp;
    /* 0x40 */ u8 mp;
    /* 0x41 */ u8 hpGrowth; /* what a partner's max HP grows by a level */
    /* 0x42 */ u8 mpGrowth;
    /* 0x43 */ u8 statGrowth[6]; /* columns of the growth tables */
    /* 0x49 */ u8 resistGrowth[7]; /* 1-5: how fast the gyms raise them */
    /* 0x50 */ u8 blastForms[5]; /* what a blast turns a partner into (from 1), by its level's tier */
    /* 0x55 */ u8 nameId; /* string in file 0x4F */
    /* 0x56 */ u8 family; /* the family it fights as (BattleStats.family) */
} DigimonData;

/* A technique (TECHS, from 1: technique n is TECHS[n - 1]) */
typedef struct TechData {
    /* 0x00 */ u16 mp; /* its cost */
    /* 0x02 */ u16 power; /* its damage, times the user's attack over the target's defense */
    /* 0x04 */ u8 icon; /* a frame of the menu sprites, from 0x37 */
    /* 0x05 */ u8 kind; /* 3: heals the target */
    /* 0x06 */ u8 accuracy;
    /* 0x07 */ u8 element; /* ELEMENT_FIRST and up, under it none */
    /* 0x08 */ u8 elementPower; /* how much the element adds, against the target's resistance */
    /* 0x09 */ u8 family; /* FAMILY_FIRST and up, under it none */
    /* 0x0A */ u8 effect; /* TECH_EFFECT_* (fightstg/battle.h), under TECH_EFFECT_FIRST none */
    /* 0x0B */ u8 effectChance;
    /* 0x0C */ u8 effectPower; /* the effect's strength (a boost's amount, a drain's 128ths) */
    /* 0x0D */ u8 scriptStage; /* its script's stage (BattleScript.stage) */
    /* 0x0E */ u8 scriptEffect; /* its script's sprite effect (BattleScript.effect) */
    /* 0x0F */ u8 scriptSound; /* its script's hit sound (BattleScript.sound) */
    /* 0x10 */ u8 script; /* its script in the user's archive (BattleScript.index); 5 and 12 are special */
    /* 0x11 */ u8 hitCount;
} TechData;

/*
 * Where an equipment item goes (its record's kind), as the shop's and
 * STSTATUS's strings name the kinds: the slots of PartnerStats.equip are
 * 0 head, 1 body, 2 right hand, 3 left hand, 4 and 5 accessories
 */
#define EQUIP_KIND_RIGHT_HAND 1
#define EQUIP_KIND_LEFT_HAND 2
#define EQUIP_KIND_EITHER_HAND 3
#define EQUIP_KIND_HEAD 4
#define EQUIP_KIND_BODY 5
#define EQUIP_KIND_ACCESSORY 6
#define EQUIP_KIND_BOTH_HANDS 7 /* takes slots 2 and 3 */
#define EQUIP_KIND_GROUP_ACCESSORY 8 /* one of its group at a time */

/* ItemInfo.data's record for a weapon (types 2-14, WEAPON_DATA) */
typedef struct WeaponData {
    /* 0x00 */ s16 charisma; /* what it adds to STAT_CHARISMA (computeStats); armour and accessories too */
    /* 0x02 */ u8 kind; /* EQUIP_KIND_* */
    /* 0x03 */ u8 group;
    /* 0x04 */ u8 partners; /* 1 << partner for each partner who can equip it (STSTATUS_canEquip) */
    /* 0x06 */ u16 amounts[2]; /* what it adds to stats[] */
    /* 0x0A */ u16 atk;
    /* 0x0C */ u8 stats[2];
    /* 0x0E */ u8 accuracy; /* added to the battle's (FIGHTSTG_computeStats) */
    /* 0x0F */ u8 hitEffect; /* 1 none, 2 poison, 3 paralysis, 4 confusion, 6 knock-out, 8 drain,
                                11 criticals; nothing reads it: FIGHTSTG_computeStats tests the
                                items' ids */
    /* 0x10 */ u8 statusChance; /* of the status its hits inflict */
    /* 0x11 */ u8 statusPower; /* the status's, or the critical bonus */
    /* 0x12 */ u8 family; /* its hits': FAMILY_FIRST and up, under it none */
} WeaponData;

/* An armour's (types 15-20, ARMOR_DATA) */
typedef struct ArmorData {
    /* 0x00 */ s16 charisma;
    /* 0x02 */ u8 kind;
    /* 0x03 */ u8 group;
    /* 0x04 */ u8 partners;
    /* 0x06 */ u16 amounts[2];
    /* 0x0A */ u8 stats[2];
    /* 0x0C */ s16 def;
    /* 0x0E */ u8 padE[2]; /* 0 on every armour */
    /* 0x10 */ u8 evasion; /* added to the battle's (FIGHTSTG_computeStats) */
    /* 0x11 */ u8 unused11; /* 1 on every armour; nothing reads it */
    /* 0x12 */ u8 pad12[2]; /* 0 on every armour */
} ArmorData;

/* An accessory's (types 21-24, ACCESSORY_DATA) */
typedef struct AccessoryData {
    /* 0x0 */ s16 charisma;
    /* 0x2 */ u8 kind; /* EQUIP_KIND_ACCESSORY or EQUIP_KIND_GROUP_ACCESSORY */
    /* 0x3 */ u8 group;
    /* 0x4 */ u8 partners;
    /* 0x6 */ u16 amount;
    /* 0x8 */ u8 stat;
    /* 0x9 */ u8 pad[3]; /* 0 on every accessory */
} AccessoryData;

/* ItemInfo.data's record, by ItemInfo.type */
typedef union ItemData {
    WeaponData weapon;
    ArmorData armor;
    AccessoryData acc;
} ItemData;

/* ItemEffect.flags */
#define ITEM_USE_MENU 1 /* usable from the status menu (STSTATUS) */
#define ITEM_USE_BATTLE 2 /* usable in battle (FIGHTSTG's item menu) */
#define ITEM_CARD_PACK 4 /* on the card boosters, which nothing tests it on */

/* What a usable item does (its ItemInfo.data, an ITEM_EFFECT_*) */
typedef struct ItemEffect {
    /* 0x0 */ u8 flags; /* ITEM_USE_* */
    /* 0x1 */ u8 kind; /* 1 heals amount HP, 2-17 raise a stat by up to amount (STSTATUS_useItem) */
    /* 0x2 */ u16 amount; /* the HP it heals, the gauge it fills, a boost in 128ths */
} ItemEffect;

/* What ItemInfo.data points to: a weapon's, an armour's or an accessory's
   record, or a usable item's effect. The overlays read both byte by byte. */
typedef union ItemRecord {
    u8 *bytes;
    ItemData *record;
    ItemEffect *effect;
    WeaponData *weapon; /* WEAPON_DATA's, for ITEMS */
    ArmorData *armor;
    AccessoryData *acc;
} ItemRecord;

/* ItemInfo.kind: the item list (ITEM_LISTS) an item is in, from 1 */
#define ITEM_KIND_KEY 1 /* the key items, and the ids no item uses */
#define ITEM_KIND_USABLE 2
#define ITEM_KIND_WEAPON 3
#define ITEM_KIND_ARMOR 4
#define ITEM_KIND_ACCESSORY 5

/* An item (ITEMS, ITEM_FUNCS) */
typedef struct ItemInfo {
    /* 0x0 */ ItemRecord data; /* the type's record, or the usable items' effect */
    /* 0x4 */ u16 price;
    /* 0x6 */ u16 sellPrice; /* 0: cannot be sold */
    /* 0x8 */ u8 kind; /* ITEM_KIND_* (ITEM_FUNCS.isKind) */
    /* 0x9 */ u8 type; /* 2-14 weapons, 15-20 armour, 21-24 accessories */
} ItemInfo;

/* ItemInfo.type's ranges: 2-14 weapons, 15-20 armour, 21-24 accessories,
   each tested as one unsigned byte compare */
#define IS_WEAPON_TYPE(type) ((u8)((type) - 2) < 13)
#define IS_ARMOR_TYPE(type) ((u8)((type) - 15) < 6)
#define IS_ACCESSORY_TYPE(type) ((u8)((type) - 21) < 4)

/* The Digimon a partner can have, and the ones it takes to battle */
#define PARTNER_ENTRY_COUNT 44
#define PARTNER_SLOT_COUNT 3

/* The first PartnerEntry.id of a Digimon: 0 is a free entry, 1 and 2 unused ones */
#define FIRST_ENTRY_ID 3

/* One of a partner's Digimon (getPartnerEntry, setPartnerEntry) */
typedef struct PartnerEntry {
    /* 0x00 */ s16 id; /* FIRST_ENTRY_ID and up */
    /* 0x02 */ s8 level; /* shown in the lab; 1 when added */
    /* 0x04 */ s32 exp;
    /* 0x08 */ s16 skills[6]; /* SKILL_ID and the flags below, 0 for none */
} PartnerEntry;

/* PartnerEntry.skills */
#define SKILL_ID 0x1FFF /* the skill, from 1 (TECHS[id - 1]) */
#define SKILL_KNOWN 0x2000
#define SKILL_MARKED 0x4000 /* marked in the lab (STGDGLAB_updateSkillPanel) */
#define SKILL_LAST 0x8000 /* the sixth skill */

/* The cards of a deck, and the decks the player has */
#define DECK_SIZE 40
#define DECK_COUNT 3

/* The copies of a card the player can have */
#define CARD_COPIES_MAX 9

/* A partner's equipment set: the four items computeStats compares with equip[0-3] */
typedef struct EquipSet {
    s16 items[4];
} EquipSet;

/* A card deck */
typedef struct Deck {
    /* 0x00 */ char name[0x16];
    /* 0x16 */ s16 cards[DECK_SIZE];
} Deck;

/* Indices of a partner's stats (PartnerStats.stats, computeStats, setStat) */
#define STAT_LEVEL 0
#define STAT_TP 1 /* what the training's intensities cost */
#define STAT_HP 2
#define STAT_MAX_HP 3
#define STAT_MP 4
#define STAT_MAX_MP 5
/* the six battle stats */
#define STAT_STRENGTH 6
#define STAT_DEFENSE 7
#define STAT_SPIRIT 8
#define STAT_WISDOM 9
#define STAT_SPEED 10
#define STAT_CHARISMA 11
#define STAT_RESISTS 12 /* the first of the seven resistances */
#define STAT_COUNT 19

/*
 * A partner Digimon from its name on (Partner.info), as getPartnerStats gives
 * it. Stats (computeStats and setStat order): 0 level, 1 TP, 2 HP, 3 max HP,
 * 4 MP, 5 max MP, 6-11 the battle stats strength, defense, spirit, wisdom,
 * speed and charisma (STSTATUS's messages for the items that raise them;
 * weapons raise 6, armour 7, all equipment 11), 12-18 seven resistances;
 * the totals add 19-21, which are subtracted from 6, 7 and 10.
 */
typedef struct PartnerStats {
    /* 0x000 */ char name[0x18];
    /* 0x018 */ s32 exp;
    /* 0x01C */ s16 stats[STAT_COUNT];
    /* 0x042 */ s16 status[3];
    /* 0x048 */ s16 slots[4]; /* PARTNER_SLOT_COUNT entries picked from entries[] */
    /* 0x050 */ PartnerEntry entries[PARTNER_ENTRY_COUNT];
    /* 0x3C0 */ s16 equip[6];
    /* 0x3CC */ u8 lastBonus; /* the training whose bonus try last worked, 0 for
                                 none: its try can't work again at once */
} PartnerStats;

/* PartnerStats.equip as a whole: STSTATUS and STITSHOP save it with a struct
   copy before they equip an item to compute the stats with it, and put it
   back */
typedef struct PartnerEquip {
    s16 items[6];
} PartnerEquip;

/* PartnerStats' stats[] and the status[] after them as one block, which
   computeStats and STITSHOP_computeStats copy whole */
typedef struct StatBlock {
    /* 0x00 */ s16 stats[STAT_COUNT];
    /* 0x26 */ s16 penalties[3]; /* subtracted from STAT_STRENGTH, STAT_DEFENSE and STAT_SPEED */
} StatBlock;

/* GameState's play time, which a memory card save copies whole */
typedef struct PlayTime {
    /* 0x0 */ s32 frames; /* 8.8, counted by the vsync callback */
    /* 0x4 */ s16 hours;
    /* 0x6 */ s16 minutes;
    /* 0x8 */ s16 seconds;
    /* 0xA */ s16 maxed; /* stopped at 999:59:59 */
} PlayTime;

/* A partner Digimon's animation in a menu: up to seven sprite ids, -1
   ending it early (the sprites are each overlay's own) */
typedef struct PartnerAnim {
    s32 frames[7];
} PartnerAnim;

/* One of the eight partner Digimon */
typedef struct Partner {
    /* 0x000 */ u8 pad[4]; /* nothing reads or writes it */
    /* 0x004 */ s32 unlocked; /* partner id + 3, 0 while locked */
    /* 0x008 */ s32 battleDigivolve; /* an entry id, compared with the slots': the Digimon the battle starts it as, 0 for its own */
    /* 0x00C */ PartnerStats info;
} Partner;

/* An enemy of a battle (FIELDSTG's encounter table points to these) */
typedef struct BattleEnemy {
    /* 0x0 */ s32 fighter; /* 0 for none */
    /* 0x4 */ s16 level;
    /* 0x6 */ s16 hp;
    /* 0x8 */ s16 mp;
    /* 0xA */ s16 strength; /* its stats and techniques' power, in 16ths */
} BattleEnemy;

/* BattleSetup.blocks' indices: the encounter keeps the player from
   inflicting a status, draining, stealing, lowering a stat of the enemy's
   or running away */
#define BATTLE_BLOCK_POISON 0
#define BATTLE_BLOCK_PARALYSIS 1
#define BATTLE_BLOCK_CONFUSION 2
#define BATTLE_BLOCK_SLEEP 3
#define BATTLE_BLOCK_KNOCK_OUT 4
#define BATTLE_BLOCK_DRAIN 5
#define BATTLE_BLOCK_STEAL 7
#define BATTLE_BLOCK_LOWER_ATTACK 8 /* to 10: attack, defense, speed (a technique's boost stat + 8) */
#define BATTLE_BLOCK_LOWER_DEFENSE 9
#define BATTLE_BLOCK_LOWER_SPEED 10
#define BATTLE_BLOCK_RUN_AWAY 11

/* The next battle: FIELDSTG_startEncounter fills it (the stage select its
   first words), FIGHTSTG and WFIGHTMN read it; the gauges carry over from
   battle to battle */
typedef struct BattleSetup {
    /* 0x00 */ s32 randomBattles; /* the field starts random battles; the stage select toggles it (Select) */
    /* 0x04 */ s32 debugUpDown; /* -1 to 3, set by the stage select (pad 2's up/down); nothing reads it */
    /* 0x08 */ s32 debugLeftRight; /* -1 to 7, set by the stage select (pad 2's left/right); nothing reads it */
    /* 0x0C */ s32 stage; /* the battle's fight stage */
    /* 0x10 */ s32 battle; /* the battle (BattleResult.battle) */
    /* 0x14 */ s32 music; /* the battle's music */
    /* 0x18 */ BattleEnemy enemies[3];
    /* 0x3C */ u8 ambushChance; /* a chance that WFIGHTMN scales by level */
    /* 0x3D */ u8 encounterKind; /* Encounter.kind; only the enemies' condition 13 reads it */
    /* 0x3E */ u8 blocks[12]; /* by BATTLE_BLOCK_*: what the player can't do in the battle */
    /* 0x4C */ s32 hasPrize; /* 1: the battle always gives prize */
    /* 0x50 */ s32 prize; /* an item (BattleResult.item) */
    /* 0x54 */ void (*clearGauges)(void);
    /* 0x58 */ s16 gauges[8]; /* filled in battle, up to 1000 */
} BattleSetup;

/* What the battle left for the report (BATTLE_RESULT, cleared by WFIGHTMN) */
typedef struct BattleResult {
    /* 0x00 */ s16 battle; /* row of STFGTREP_rewards */
    /* 0x02 */ s16 item; /* the item won, 0 for none */
    /* 0x04 */ s16 member; /* the party member whose accessory adds money */
    /* 0x06 */ struct {
        u8 fought;
        u8 used[3]; /* the Digimon of each slot was used */
    } partners[3];
} BattleResult;

/* computeStats' result: the stats with the equipment added, by STAT_* or
   by name. computeStats fills the first 22 (0x2C bytes). */
typedef union PartnerTotals {
    s16 stats[24];
    struct {
        /* 0x00 */ s16 level;
        /* 0x02 */ s16 tp; /* what the training's intensities cost */
        /* 0x04 */ s16 hp;
        /* 0x06 */ s16 maxHp;
        /* 0x08 */ s16 mp;
        /* 0x0A */ s16 maxMp;
        /* 0x0C */ s16 battle[6]; /* strength (raised by weapons), defense (by armour), spirit,
                                     wisdom, speed and charisma (by all equipment) */
        /* 0x18 */ s16 resist[7];
        /* 0x26 */ s16 lowered[3]; /* taken from battle[0], [1] and [4] */
        /* 0x2C */ s32 spare; /* not filled: STGTRAIN's result keeps its yes/no answer here, 0 yes */
    } fields;
} PartnerTotals;

/* Game modes (GameState: mode >> 8 is the overlay) that more than their own
   overlay asks for */
#define MODE_NEW_GAME 0x2D7 /* FIELDSTG, where a new game starts */
#define MODE_DECK_EDITOR 0x400 /* STCRDDEK */
#define MODE_PLAYER_NAME 0x500 /* STPLNMET: the player's name entry */
#define MODE_BATTLE 0x600 /* FIGHTSTG */
#define MODE_CARD_GAME 0x700 /* CARDGAME: a card battle */
#define MODE_TRAINING 0xA00 /* STGTRAIN */
#define MODE_NAMING 0xB00 /* STDGNAME */
#define MODE_CONTINUE 0xC00 /* STGMCARD, to load a game */
#define MODE_DIGI_LAB 0xD00 /* STGDGLAB */
#define MODE_TITLE 0xE00 /* STDWTITL's title screen; its movies follow */
#define MODE_OPENING 0xE01 /* STDWTITL's first movie */
#if VERSION_US
#define MODE_BATTLE_MOVIE 0xE09 /* STDWTITL's movie before each battle at GAME.progress 0x2B */
#define MODE_ENDING 0xE0A /* STDWTITL's movie after the last battle */
#elif VERSION_EU || VERSION_JP
#define MODE_OPENING_2 0xE02 /* STDWTITL's second opening movie: LANGUAGE 0
                                alternates the two, the others play only it */
#define MODE_BATTLE_MOVIE 0xE0A
#define MODE_ENDING 0xE0B
#endif
#define MODE_ITEM_SHOP 0xF00 /* STITSHOP */
#define MODE_STATUS 0x1000 /* STSTATUS: the field menu's screens (FIELD_MENU_CHOICE) */
#define MODE_CARD_ALBUM 0x1200 /* STCRDABM */
#define MODE_CARD_SHOP 0x1300 /* STCRDSHP */
#define MODE_BATTLE_REPORT 0x1400 /* STFGTREP */
#define MODE_STAGE_SELECT 0x1500 /* STAGSLCT, the debug stage select */
#define MODE_COUNTRY_SELECT 0x1600 /* CNTY_SEL, where the European version starts */

/* The first mode of a mode's overlay: MODE_OVERLAY(mode) == MODE_TITLE for
   all of STDWTITL's */
#define MODE_OVERLAY(mode) ((mode) & 0xFF00)

/* The partner Digimon, and the ones in the party */
#define PARTNER_COUNT 8
#define PARTY_SIZE 3

/* The most money the player can have */
#define MONEY_MAX 9999999

/*
 * The game state (GAME): the first 0x26BC bytes are what newGame clears (the
 * save data), then the current game mode and the methods.
 * Game modes: mode >> 8 selects the overlay (MODE_OVERLAY_FILES); a mode
 * change is requested with requestMode and applied by commitMode when main
 * recreates the mode task.
 */
typedef struct GameState {
    /* 0x0000 */ u8 checksum; /* a save's, of its bytes from 0x4 (STGMCARD_runSaves) */
    /* 0x0001 */ u8 pad1; /* nothing reads or writes it */
    /* 0x0002 */ u8 version; /* a save's MEMCARD_SAVE_VERSION (stgmcard.h) */
    /* 0x0003 */ u8 pad3; /* nothing reads or writes it */
    /* 0x0004 */ u8 digivolveDemo;
    /* 0x0005 */ u8 pad5[7]; /* nothing reads or writes them */
    /* 0x000C */ s32 unusedC; /* newGame sets it to -1; nothing reads it */
    /* 0x0010 */ u8 pad10[0x18]; /* nothing reads or writes them */
    /* 0x0028 */ s32 stageSelectTop; /* the debug stage select's first line */
    /* 0x002C */ s32 stageSelectCursor;
    /* 0x0030 */ s32 battleSteps; /* to the next random battle, which each step lowers */
    /* 0x0034 */ s32 fieldMode; /* where the menu returns to */
    /* 0x0038 */ Vec2 fieldPos; /* the player's, there */
    /* 0x0040 */ s32 fieldDir;
    /* 0x0044 */ u16 place; /* where the last warp or trigger put the player (FieldBattles.id) */
    /* 0x0046 */ u16 placeArg; /* the place's argument, which the stage reads with it */
    /* 0x0048 */ PlayTime playTime;
    /* 0x0054 */ char name[0x18]; /* the player's */
    /* 0x006C */ s32 money;
    /* 0x0070 */ s32 party[PARTY_SIZE]; /* partner indices */
    /* 0x007C */ s8 items[0x193]; /* counts, up to 99 */
    /* 0x020F */ s8 equippedItems[0x193];
    /* 0x03A2 */ s8 cards[0x13D]; /* counts, up to CARD_COPIES_MAX */
    /* 0x04DF */ u8 cardsSeen[0x149];
    /* 0x0628 */ Deck decks[DECK_COUNT];
    /* 0x075C */ Partner partners[PARTNER_COUNT];
    /* 0x263C */ s32 progress;
    /* 0x2640 */ s32 partySet; /* setParty's */
    /*
     * flags02-flags40: the event flag groups 0x02-0x40 (checkCondition,
     * applyAction), bitsets packed one after the other, so most start at an
     * odd byte. The Japanese version's state is the European one's: its
     * newGame clears 0x26C4 bytes and its code reads the same offsets
     */
#if VERSION_US
    /* 0x2644 */ u8 flags02[0xD];
    /* 0x2651 */ u8 flags04[0x2];
    /* 0x2653 */ u8 flags06[0x1];
    /* 0x2654 */ u8 flags08[0x1];
    /* 0x2655 */ u8 flags0A[0x2];
    /* 0x2657 */ u8 flags0C[0x8];
    /* 0x265F */ u8 flags0E[0xC];
    /* 0x266B */ u8 flags10[0x2];
    /* 0x266D */ u8 flags18[0x1];
    /* 0x266E */ u8 flags1A[0x9];
    /* 0x2677 */ u8 flags1C[0xB];
    /* 0x2682 */ u8 flags20[0x1E];
    /* 0x26A0 */ u8 flags40[0x1C];
    /* 0x26BC */ s32 mode;
    /* 0x26C0 */ s32 nextMode;
    /* 0x26C4 */ s32 prevMode;
    /* 0x26C8 */ s32 modeArg;
    /* 0x26CC */ u8 countdown[4]; /* three digits of seconds, then frames */
    /* 0x26D0 */ s32 clearTempFlags;
    /* 0x26D4 */ s32 lastFieldMode; /* the field mode FIELDSTG last started */
    /* 0x26D8 */ s32 mapIndex; /* which of the stage's maps the field uses */
    /* 0x26DC */ s32 altTriggers; /* WSTAG810: its second triggers map is in use */
    /* 0x26E0 */ s32 playerDepth; /* the player's depth, kept while in the mode */
    /* 0x26E4 */ s32 prizeSpot; /* the hidden spot that has the prize */
    /* 0x26E8 */ s32 dark; /* the Dark Dungeon's darkness is on (WSTAG745, WSTAG746) */
    /* 0x26EC */ s32 flightZ; /* the flying player's height, kept while in the mode */
    /* 0x26F0 */ GameFuncs funcs;
#elif VERSION_EU || VERSION_JP
    /* 0x2644 */ u8 flags02[0x12];
    /* 0x2656 */ u8 flags04[0x2];
    /* 0x2658 */ u8 flags06[0x1];
    /* 0x2659 */ u8 flags08[0x1];
    /* 0x265A */ u8 flags0A[0x4];
    /* 0x265E */ u8 flags0C[0x8];
    /* 0x2666 */ u8 flags0E[0xC];
    /* 0x2672 */ u8 flags10[0x4];
    /* 0x2676 */ u8 flags18[0x2];
    /* 0x2678 */ u8 flags1A[0x9];
    /* 0x2681 */ u8 flags1C[0xB];
    /* 0x268C */ u8 flags20[0x1E];
    /* 0x26AA */ u8 flags40[0x1A];
    /* 0x26C4 */ s32 mode;
    /* 0x26C8 */ s32 nextMode;
    /* 0x26CC */ s32 prevMode;
    /* 0x26D0 */ s32 modeArg;
    /* 0x26D4 */ u8 countdown[4];
    /* 0x26D8 */ s32 clearTempFlags;
    /* 0x26DC */ s32 lastFieldMode;
    /* 0x26E0 */ s32 mapIndex;
    /* 0x26E4 */ s32 altTriggers;
    /* 0x26E8 */ s32 playerDepth;
    /* 0x26EC */ s32 prizeSpot;
    /* 0x26F0 */ s32 dark;
    /* 0x26F4 */ s32 flightZ;
    /* 0x26F8 */ s32 randomGauges; /* gauge games left with random rows, reset with each new mode */
    /* 0x26FC */ GameFuncs funcs;
#endif
} GameState;

/* The saved part of the game state, GAME up to mode: a save's data section
   (stgmcard.h's GameSave) */
#if VERSION_US
#define GAME_SAVE_SIZE 0x26BC
#elif VERSION_EU || VERSION_JP
#define GAME_SAVE_SIZE 0x26C4
#endif

void newGame(void);
void commitMode(void);
s32 getPrevMode(void);
s32 getMode(void);
s32 getModeArg(void);
void requestMode(s32 mode, s32 arg);
s32 isModeChangePending(void);
s32 unequipItem(s32 slot, s32 item);
void changeItem(s32 item, s32 add);
void changeCard(s32 card, s32 add);
void startCardBattle(s32 opponent, s32 kind);
s32 checkPartner(u32 op, s32 arg);
s32 checkMoney(s32 op, s32 item);
s32 checkProgressRange(s32 unused, s32 index);
s32 checkFlagCount(s32 unused, s32 mode);
s32 showActorIcon(s32 op, s32 arg);
s32 checkSpecialCondition(s32 id, s32 expected);
s32 checkProgress(s32 value, s32 mode);
s32 checkItem(s32 index, s32 mode);
s32 checkCard(s32 card, s32 have);
s32 checkPartyStat(s32 index, s32 mode);
s32 checkWarpArg(s32 id, s32 mode);
s32 findDigimon(s32 id);
void clearBattleGauges(void);
ItemInfo *getItem(s32 id);
s32 getItemCategory(s32 id);
s32 isItemKind(s32 item, s32 kind);
s32 listItems(s32 type, s16 *out);
DigimonData *getDigimon(s32 id);
void initNewGameData(void);
void addCards(s32 card, s32 count);
s32 findPartnerEntry(s32 slot, s32 id);
void applyAction(s32 code, s32 value);
s32 checkConditions(u16 *list);
void applyActions(u16 *list);
void updateModeFlags(void);
s32 checkCondition(u16, u16);
s32 testBit(u8 *bits, s32 index, s32 set);
void setBit(u8 *bits, s32 index, s32 set);
s32 checkItemSet(s32 op, s32 arg);
s32 getPartnerSlots(s32 partner, s16 *out);
void setPartnerSlots(s32 partner, s16 *ids);
s32 listPartnerEntries(s32 partner, u16 *out);
s32 addPartnerEntry(s32 partner, s32 id);
s32 getPartnerEntry(s32 partner, s32 id, PartnerEntry *out);
s32 setPartnerEntry(s32 partner, s32 id, PartnerEntry *in);
void addStatBonus(s16 *p, s32 stat, s32 delta);
PartnerStats *getPartnerStats(s32 partner);
s32 getPartyMember(u32 index);
void setParty(s32 set);
void giveStarterDeck(void);
void resetPlayTime(void);
void updatePlayTime(void);
s32 getPartyPartner(u32 index);
void setStat(s32 partner, u32 stat, s16 value);
void addStat(s32 partner, u32 stat, s32 delta);
void computeStats(s32 partner, PartnerTotals *out);

extern DigimonData DIGIMON_DATA[];
extern DigimonData *(*GET_DIGIMON)(s32 id); /* getDigimon */
extern BattleSetup BATTLE_SETUP;
extern BattleResult BATTLE_RESULT;
extern ItemInfo ITEMS[];
extern TechData TECHS[];

/* A special condition (SPECIAL(id)): checkSpecialCondition runs the check
   function of kind `check >> 4` (checkItemSet, checkPartner, ...) with
   `check & 0xF` and arg */
typedef struct SpecialCondition {
    /* 0x0 */ u8 id;
    /* 0x1 */ u8 check;
    /* 0x2 */ u8 arg;
} SpecialCondition;

/* The item functions (ITEM_FUNCS) */
typedef struct ItemFuncs {
    /* 0x0 */ struct ItemInfo *(*get)(s32 item); /* getItem */
    /* 0x4 */ s32 (*getCategory)(s32 item); /* getItemCategory */
    /* 0x8 */ s32 (*isKind)(s32 item, s32 kind); /* ItemInfo.kind == kind */
    /* 0xC */ s32 (*list)(s32 type, s16 *out); /* listItems */
} ItemFuncs;
extern ItemFuncs ITEM_FUNCS;
extern u8 ITEM_TYPE_CATEGORIES[];
extern s32 MONEY_REQUIRED[];
extern SpecialCondition SPECIAL_CONDITIONS[];
extern s32 MONEY_GAINS[];
extern s32 MONEY_LOSSES[];
extern GameFlags FLAGS_00;
extern s32 STARTER_DECK[DECK_SIZE];
extern u8 STARTER_PARTIES[][3];
extern u8 PROGRESS_RANGES[][2];
extern s32 PARTY_STAT_THRESHOLDS[];
extern EquipSet EQUIP_SETS[];
extern s16 EQUIP_SET_BONUSES[][6];
extern u16 *ITEM_LISTS[]; /* listItems' lists, 0-terminated */
extern GameState GAME;

#endif /* DW3_GAME_STATE_H */
