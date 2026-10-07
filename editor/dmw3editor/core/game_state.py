"""Named game constants identified by the upstream decompilation.

Naming and documentation only — this module is a *dictionary*, not behaviour.
It is imported by no runtime path (the save/checksum/UI code does not use it);
it exists so a future story-progress / event-flag / technique editor has the
game's own names and tables on hand, and so the offsets the editor already
relies on can be cited against the decomp instead of guessed.

Authority: ``ReGame-Labs/dw3_decomp`` @ ``5d7ca79`` ("Name the shared modes,
screen, palettes, sounds, condition codes and technique fields ..."). Every
value below is copied from these files, cited inline:

  * ``include/dw3/game_state.h`` — GAME, the flag bitsets, the event-code
    macros, TechData, Partner/PartnerStats, the MODE_* defines.
  * ``src/main/game/events.c`` — the condition/action decoder and its tables
    (SPECIAL_CONDITIONS, PROGRESS_RANGES, PARTY_STAT_THRESHOLDS, MONEY_*).
  * ``include/dw3/memcard.h`` / ``include/stgmcard.h`` — memory-card constants
    and the MemCardSave field names (``area`` at +0x18, ``place`` at +0x1C).
  * ``include/fightstg.h`` — battle stat/element/family/element, fighter
    condition and technique-effect codes.

Offsets are UNCHANGED. The upstream tip just put names on bytes the editor
already reads at the same addresses (verified byte-identical on both sample
cards); a save's data section 0 is GAME at payload ``0x0300``, so
``payload = DATA_SECTION_BASE + game_offset``.

This module deliberately has no side effects and no imports from the editor's
model, except one import-time assertion that its base matches ``save.py``.
"""

from __future__ import annotations

from . import save as _save

# --------------------------------------------------------------------------
# The saved game state (GAME) in a data section.
# --------------------------------------------------------------------------
# game_state.h:355-358: "the first 0x26BC bytes are what newGame clears (the
# save data)". A save's data section 0 begins at payload 0x0300
# (save.DATA_SECTION_OFFSETS[0]) and holds GAME from offset 0.
DATA_SECTION_BASE = 0x0300
assert DATA_SECTION_BASE == _save.DATA_SECTION_OFFSETS[0], (
    "game_state.py is pinned to save.DATA_SECTION_OFFSETS[0]"
)

# GAME_SAVE_SIZE (game_state.h:459-463), region-specific; == save.py's map.
GAME_SAVE_SIZE_BY_VERSION = {3: 0x26BC, 4: 0x26C4}
assert GAME_SAVE_SIZE_BY_VERSION == _save.GAME_SAVE_SIZE_BY_VERSION

# GAME fields the overlay/field code reads, by their upstream names
# (game_state.h:361-455). Only the ones a save actually holds/uses are listed;
# offsets are unchanged from the editor's measured model.
GAME_FIELDS = {
    # name: (game_offset, type)  -- citations on each line
    "digivolveDemo": (0x0004, "s8"),          # game_state.h:363
    "stageSelectTop": (0x0028, "s32"),        # game_state.h:367
    "stageSelectCursor": (0x002C, "s32"),     # game_state.h:368
    "battleSteps": (0x0030, "s32"),           # game_state.h:369 (was unk30)
    "fieldMode": (0x0034, "s32"),             # game_state.h:370
    "fieldPos": (0x0038, "Vec2"),             # game_state.h:371
    "fieldDir": (0x0040, "s32"),              # game_state.h:372
    "place": (0x0044, "u16"),                 # game_state.h:373 (was unk44)
    "placeArg": (0x0046, "u16"),              # game_state.h:374 (was unk46)
    "playFrames": (0x0048, "s32"),            # game_state.h:375
    "playHours": (0x004C, "s16"),             # game_state.h:376
    "playMinutes": (0x004E, "s16"),           # game_state.h:377
    "playSeconds": (0x0050, "s16"),           # game_state.h:378
    "playTimeMaxed": (0x0052, "s16"),         # game_state.h:379
    "name": (0x0054, "char[0x18]"),           # game_state.h:380
    "money": (0x006C, "s32"),                 # game_state.h:381
    "party": (0x0070, "s32[3]"),              # game_state.h:382
    "items": (0x007C, "s8[0x193]"),           # game_state.h:383
    "equippedItems": (0x020F, "s8[0x193]"),   # game_state.h:384
    "cards": (0x03A2, "s8[0x13D]"),           # game_state.h:385
    "cardsSeen": (0x04DF, "u8[0x149]"),       # game_state.h:386
    "decks": (0x0628, "Deck[3]"),             # game_state.h:387
    "partners": (0x075C, "Partner[8]"),       # game_state.h:389
    "progress": (0x263C, "s32"),              # game_state.h:390
    "partySet": (0x2640, "s32"),              # game_state.h:391
    # game_state.h:411-423 (US). EUR is +8 from lastFieldMode on (see :439-453).
    "mode": (0x26BC, "s32"),                  # game_state.h:411
    "nextMode": (0x26C0, "s32"),              # game_state.h:412
    "prevMode": (0x26C4, "s32"),              # game_state.h:413
    "modeArg": (0x26C8, "s32"),               # game_state.h:414
    "countdown": (0x26CC, "u8[4]"),           # game_state.h:415
    "clearTempFlags": (0x26D0, "s32"),        # game_state.h:416
    "lastFieldMode": (0x26D4, "s32"),         # game_state.h:417 (was unk26D4)
    "mapIndex": (0x26D8, "s32"),              # game_state.h:418 (was unk26D8)
    "playerDepth": (0x26E0, "s32"),           # game_state.h:420 (was unk26E0)
    "prizeSpot": (0x26E4, "s32"),             # game_state.h:421 (was unk26E4)
    "flightZ": (0x26EC, "s32"),               # game_state.h:423 (was unk26EC)
}

# The two fields whose EUR offsets differ from the US ones.
GAME_FIELDS_EUR_ONLY_DELTA = {
    # name: game_offset  -- EUR (game_state.h:439-453)
    "lastFieldMode": 0x26DC,
    "mapIndex": 0x26E0,
    "playerDepth": 0x26E8,
    "prizeSpot": 0x26EC,
    "flightZ": 0x26F4,
}
RANDOM_GAUGES_EUR = 0x26F8  # game_state.h:452, EUR only (random gauge games)

# Payload offsets (data section 0) of the two story-progress fields the docs
# now state, DERIVED from GAME_FIELDS so they cannot drift.
PROGRESS_PAYLOAD = DATA_SECTION_BASE + GAME_FIELDS["progress"][0]   # 0x293C
PARTY_SET_PAYLOAD = DATA_SECTION_BASE + GAME_FIELDS["partySet"][0]  # 0x2940

# --------------------------------------------------------------------------
# Game modes (game_state.h:317-341). mode >> 8 selects the overlay.
# --------------------------------------------------------------------------
MODE_NEW_GAME = 0x2D7          # FIELDSTG, where a new game starts
MODE_DECK_EDITOR = 0x400       # STCRDDEK
MODE_PLAYER_NAME = 0x500       # STPLNMET
MODE_BATTLE = 0x600            # FIGHTSTG
MODE_CARD_GAME = 0x700         # CARDGAME: a card battle
MODE_TRAINING = 0xA00          # STGTRAIN
MODE_NAMING = 0xB00            # STDGNAME
MODE_CONTINUE = 0xC00          # STGMCARD, to load a game
MODE_DIGI_LAB = 0xD00          # STGDGLAB
MODE_TITLE = 0xE00             # STDWTITL title screen
MODE_OPENING = 0xE01           # STDWTITL first movie
MODE_BATTLE_MOVIE = 0xE09      # US (0xE0A on EUR); before each battle at progress 0x2B
MODE_ENDING = 0xE0A            # US (0xE0B on EUR); after the last battle
MODE_ITEM_SHOP = 0xF00         # STITSHOP
MODE_STATUS = 0x1000           # STSTATUS
MODE_CARD_ALBUM = 0x1200       # STCRDABM
MODE_CARD_SHOP = 0x1300        # STCRDSHP
MODE_BATTLE_REPORT = 0x1400    # STFGTREP
MODE_STAGE_SELECT = 0x1500     # STAGSLCT (debug)
MODE_COUNTRY_SELECT = 0x1600   # CNTY_SEL (EUR start)


def MODE_OVERLAY(mode: int) -> int:
    """The first mode of a mode's overlay (game_state.h:345)."""
    return mode & 0xFF00


# --------------------------------------------------------------------------
# The event code system (game_state.h:66-78; events.c checkCondition/applyAction)
# --------------------------------------------------------------------------
# A script tests/applies (code, value) pairs; code >> 8 & 0xFE is the "group",
# code & 0x1FF the flag/item. The upstream macros, reproduced as functions.
CODES_END = 0xFFFF


def FLAG(group: int, id: int) -> int:
    """A flag bitset bit: group 0x00 (FLAGS_00) .. 0x40 (GAME.flags40)."""
    return (group << 8) | id


def PROGRESS(n: int) -> int:
    """GAME.progress is ``n``."""
    return 0x6000 | n


def SPECIAL(id: int) -> int:
    """SPECIAL_CONDITIONS entry ``id``."""
    return 0x7000 | id


def PARTY_STAT(id: int) -> int:
    """checkPartyStat (group 0x72)."""
    return 0x7200 | id


def EVENT_BATTLE(id: int) -> int:
    """action: FIELDSTG_battleFuncs.startEventBattle (group 0x74)."""
    return 0x7400 | id


def CARD_BATTLE(opponent: int, kind: int) -> int:
    """action: startCardBattle (groups 0x76/0x78)."""
    return 0x7600 + kind * 0x200 | opponent


def WARP_ARG(id: int) -> int:
    """checkWarpArg (group 0x7E): GAME.place / GAME.placeArg."""
    return 0x7E00 | id


def ITEM(kind: int, id: int) -> int:
    """An item in the bag or equipped (groups 0x80-0x8E).

    ``kind`` (bits 9-11) is not read by the decoder; the scripts set it loosely
    after the item's kind.
    """
    return 0x8000 | kind << 9 | id


def START_EVENT(index: int) -> int:
    """action: FIELDSTG_startListedEvent (group 0x90)."""
    return 0x9000 | index


def CARD(id: int) -> int:
    """A card: the player has one, or gets or loses one (group 0x92)."""
    return 0x9200 | id


# The event groups a (code, value) pair can belong to, by decode (events.c:507).
EVENT_GROUP_FLAGS_00 = 0x00
EVENT_GROUPS_FLAG_BITSETS = (0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E,
                             0x10, 0x18, 0x1A, 0x1C, 0x20, 0x40)
EVENT_GROUP_PROGRESS = 0x60
EVENT_GROUP_SPECIAL = 0x70
EVENT_GROUP_PARTY_STAT = 0x72
EVENT_GROUP_WARP_ARG = 0x7E
EVENT_GROUPS_ITEM = (0x80, 0x82, 0x84, 0x86, 0x88, 0x8A, 0x8C, 0x8E)
EVENT_GROUP_CARD = 0x92

# Action-only groups (applyAction, events.c:561): mode changes through the
# overlay, shops, the lab / naming screens, an event battle, card battles.
ACTION_GROUP_MODE_HOOK_74 = 0x74    # startEventBattle
ACTION_GROUP_CARD_BATTLE_76 = 0x76  # startCardBattle kind 0
ACTION_GROUP_CARD_BATTLE_78 = 0x78  # startCardBattle kind 1
ACTION_GROUP_SHOP_7A = 0x7A         # item shop / card shop / inn
ACTION_GROUP_SCREEN_7C = 0x7C       # digi lab (0) / naming (1)
ACTION_GROUP_START_EVENT_90 = 0x90  # FIELDSTG_startListedEvent
ACTION_GROUP_TRAINING_94 = 0x94     # MODE_TRAINING


def event_group(code: int) -> int:
    """The group of a condition code (events.c:508 ``(code >> 8) & 0xFE``)."""
    return (code >> 8) & 0xFE


def action_group(code: int) -> int:
    """The group of an action code (events.c:562 ``(code >> 8) & ~1``)."""
    return (code >> 8) & ~1


def event_code_id(code: int) -> int:
    """The flag/item id of a code (events.c:509 ``code & 0x1FF``)."""
    return code & 0x1FF


# --------------------------------------------------------------------------
# The flag bitsets — groups 0x02..0x40 (game_state.h:397-410 US / 425-438 EUR)
# --------------------------------------------------------------------------
# Group 0x00 (FLAGS_00) is GameFlags.bits[4] in RAM, NOT part of the save.
# Each row: group -> (field name, US size, EUR size, US game offset, EUR game
# offset). Sizes differ per region; the offsets are byte-identical to the
# editor's FORBIDDEN_REGIONS-straddling area (payload 0x2944..0x29BC US).
FLAG_GROUPS = {
    0x02: ("flags02", 0x0D, 0x12, 0x2644, 0x2644),
    0x04: ("flags04", 0x02, 0x02, 0x2651, 0x2656),
    0x06: ("flags06", 0x01, 0x01, 0x2653, 0x2658),
    0x08: ("flags08", 0x01, 0x01, 0x2654, 0x2659),
    0x0A: ("flags0A", 0x02, 0x04, 0x2655, 0x265A),
    0x0C: ("flags0C", 0x08, 0x08, 0x2657, 0x265E),
    0x0E: ("flags0E", 0x0C, 0x0C, 0x265F, 0x2666),
    0x10: ("flags10", 0x02, 0x04, 0x266B, 0x2672),
    0x18: ("flags18", 0x01, 0x02, 0x266D, 0x2676),
    0x1A: ("flags1A", 0x09, 0x09, 0x266E, 0x2678),
    0x1C: ("flags1C", 0x0B, 0x0B, 0x2677, 0x2681),
    0x20: ("flags20", 0x1E, 0x1E, 0x2682, 0x268C),
    0x40: ("flags40", 0x1C, 0x1A, 0x26A0, 0x26AA),
}


def flag_group_payload_offset(group: int, version: int = 3) -> int:
    """Payload offset of a flag group's first byte in data section 0.

    ``version`` is the save's version byte (3 = USA, 4 = EUR). Raises KeyError
    for group 0x00 (RAM-only) or an unknown group.
    """
    name, _us, _eu, us_off, eu_off = FLAG_GROUPS[group]
    off = eu_off if version == 4 else us_off
    return DATA_SECTION_BASE + off


def flag_group_size(group: int, version: int = 3) -> int:
    """Byte size of a flag group's bitset on a region (3 = USA, 4 = EUR)."""
    _name, us, eu, _uo, _eo = FLAG_GROUPS[group]
    return eu if version == 4 else us


# Derived story-progress landmarks (the docs state these explicitly).
FLAG_GROUPS_PAYLOAD_SPAN = (
    flag_group_payload_offset(0x02, 3),                         # 0x2944
    DATA_SECTION_BASE + GAME_FIELDS["mode"][0],                 # 0x29BC (= GAME.mode)
)

# updateModeFlags clears FLAGS_00 bits 2..0 and sets the card-battle flags
# 0x10-0x12 (events.c:676-696) — action code 0x10/0x11/0x12, group 0x00.
TEMP_FLAGS_CLEARED = (0x00, 0x01, 0x02)   # FLAGS_00.bits[2..0] = 0
CARD_BATTLE_FLAGS = (0x10, 0x11, 0x12)

# --------------------------------------------------------------------------
# The special-condition table (events.c:704-757) and its kinds
# --------------------------------------------------------------------------
# SPECIAL_CONDITIONS is {id, kind << 4 | op, arg}, terminated by 0xFF
# (events.c:289 ``for (p = SPECIAL_CONDITIONS; *p != 0xFF; p += 3)``).
SPECIAL_KIND_ITEM_SET = 0x00      # checkItemSet (events.c:29; EUR adds a set)
SPECIAL_KIND_PARTNER = 0x10       # checkPartner (events.c:65)
SPECIAL_KIND_MONEY = 0x20         # checkMoney (events.c:155)
SPECIAL_KIND_PROGRESS_RANGE = 0x30  # checkProgressRange (events.c:181)
SPECIAL_KIND_FLAG_COUNT = 0x40    # checkFlagCount (events.c:195)
SPECIAL_KIND_ACTOR_ICON = 0x50    # showActorIcon (events.c:275)

# Raw US table, byte-for-byte from events.c:706-729 (346 bytes incl. the 0xFF).
_SPECIAL_CONDITIONS_US_HEX = (
    0x3A, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x10, 0x01, 0x02, 0x10, 0x02, 0x10, 0x11, 0x00, 0x12,
    0x11, 0x01, 0x11, 0x11, 0x02, 0x0B, 0x11, 0x03, 0x0D, 0x11, 0x04, 0x0C, 0x11, 0x05, 0x0F, 0x11,
    0x06, 0x0E, 0x11, 0x07, 0x42, 0x12, 0x00, 0x44, 0x12, 0x01, 0x43, 0x12, 0x02, 0x47, 0x12, 0x03,
    0x46, 0x12, 0x04, 0x45, 0x12, 0x05, 0x49, 0x12, 0x06, 0x48, 0x12, 0x07, 0x37, 0x13, 0x00, 0x39,
    0x13, 0x01, 0x38, 0x13, 0x02, 0x31, 0x13, 0x03, 0x32, 0x13, 0x04, 0x33, 0x13, 0x05, 0x35, 0x13,
    0x06, 0x34, 0x13, 0x07, 0x4A, 0x14, 0x00, 0x4C, 0x14, 0x01, 0x4B, 0x14, 0x02, 0x4F, 0x14, 0x03,
    0x4E, 0x14, 0x04, 0x4D, 0x14, 0x05, 0x51, 0x14, 0x06, 0x50, 0x14, 0x07, 0x25, 0x15, 0x00, 0x26,
    0x15, 0x01, 0x27, 0x15, 0x02, 0x28, 0x15, 0x03, 0x29, 0x15, 0x04, 0x2E, 0x16, 0x00, 0x30, 0x16,
    0x01, 0x2F, 0x16, 0x02, 0x2A, 0x16, 0x03, 0x2B, 0x16, 0x04, 0x2C, 0x16, 0x05, 0x36, 0x16, 0x06,
    0x2D, 0x16, 0x07, 0x74, 0x20, 0x00, 0x75, 0x20, 0x01, 0x76, 0x20, 0x02, 0x77, 0x20, 0x03, 0x78,
    0x20, 0x04, 0x79, 0x20, 0x05, 0x7A, 0x20, 0x06, 0x7B, 0x20, 0x07, 0x7C, 0x20, 0x08, 0x7D, 0x20,
    0x09, 0x8B, 0x21, 0x00, 0x8C, 0x21, 0x01, 0x8D, 0x21, 0x02, 0x8E, 0x21, 0x03, 0x8F, 0x21, 0x04,
    0x90, 0x21, 0x05, 0x91, 0x21, 0x06, 0x92, 0x21, 0x07, 0x7E, 0x22, 0x00, 0x7F, 0x22, 0x01, 0x80,
    0x22, 0x02, 0x81, 0x22, 0x03, 0x82, 0x22, 0x04, 0x83, 0x22, 0x05, 0x84, 0x22, 0x06, 0x85, 0x22,
    0x07, 0x86, 0x22, 0x08, 0x87, 0x22, 0x09, 0x06, 0x30, 0x00, 0x03, 0x30, 0x01, 0x0A, 0x30, 0x02,
    0x07, 0x30, 0x03, 0x04, 0x30, 0x04, 0x05, 0x30, 0x05, 0x09, 0x30, 0x06, 0x14, 0x30, 0x07, 0x08,
    0x30, 0x08, 0x3F, 0x30, 0x09, 0x40, 0x30, 0x0A, 0x41, 0x30, 0x0B, 0x93, 0x30, 0x0C, 0x94, 0x30,
    0x0D, 0x15, 0x30, 0x0E, 0x16, 0x30, 0x0F, 0x17, 0x30, 0x10, 0x18, 0x30, 0x11, 0x19, 0x30, 0x12,
    0x1A, 0x30, 0x13, 0x1C, 0x30, 0x14, 0x1D, 0x30, 0x15, 0x21, 0x30, 0x16, 0x1E, 0x30, 0x17, 0x1F,
    0x30, 0x18, 0x20, 0x30, 0x19, 0x22, 0x30, 0x1A, 0x23, 0x30, 0x1B, 0x24, 0x30, 0x1C, 0x3B, 0x30,
    0x1D, 0x3C, 0x30, 0x1E, 0x3D, 0x30, 0x1F, 0x3E, 0x30, 0x20, 0x71, 0x40, 0x00, 0x70, 0x40, 0x01,
    0x73, 0x40, 0x02, 0x72, 0x40, 0x03, 0x13, 0x50, 0x00, 0xFF,
)
assert _SPECIAL_CONDITIONS_US_HEX[-1] == 0xFF
assert (len(_SPECIAL_CONDITIONS_US_HEX) - 1) % 3 == 0


def _decode_special(blob: tuple) -> tuple:
    rows = []
    for i in range(0, len(blob) - 1, 3):
        cid, kindop, arg = blob[i], blob[i + 1], blob[i + 2]
        rows.append((cid, (kindop & 0xF0) >> 4, kindop & 0x0F, arg))
    return tuple(rows)


# (id, kind nibble, op, arg); kind = SPECIAL_KIND_* >> 4.
SPECIAL_CONDITIONS = _decode_special(_SPECIAL_CONDITIONS_US_HEX)

# checkProgressRange / checkPartyStat tables (events.c:771-783).
PROGRESS_RANGES = (
    (0x02, 0x12), (0x04, 0x17), (0x04, 0x2C), (0x14, 0x17), (0x18, 0x25), (0x27, 0x2A),
    (0x04, 0x63), (0x07, 0x63), (0x18, 0x63), (0x16, 0x2A), (0x12, 0x63), (0x14, 0x63),
    (0x0F, 0x63), (0x1E, 0x63), (0x05, 0x0A), (0x0F, 0x12), (0x14, 0x16), (0x18, 0x1A),
    (0x1B, 0x25), (0x27, 0x28), (0x25, 0x26), (0x1B, 0x24), (0x22, 0x26), (0x1B, 0x26),
    (0x1C, 0x25), (0x1B, 0x21), (0x04, 0x26), (0x04, 0x15), (0x0E, 0x26), (0x11, 0x26),
    (0x0A, 0x0D), (0x1D, 0x26), (0x0C, 0x16), (0x00, 0x00),
)
PARTY_STAT_THRESHOLDS = (60, 150, 210, 285, 378, 492, 630, 795, 990, 1218,
                         1482, 1785, 2049, 2277, 2472)

# checkMoney's amounts (events.c:760-768).
MONEY_REQUIRED = (800, 1600, 2700, 4000, 6000, 8700, 11500, 17500, 24000, 32000)
MONEY_GAINS = (100, 300, 600, 1000, 1600, 3000, 5000, 8500)
MONEY_LOSSES = (800, 1600, 2700, 4000, 6000, 8700, 11500, 17500, 24000, 32000)

# --------------------------------------------------------------------------
# Techniques (TechData, game_state.h:114-132). Offsets unchanged.
# --------------------------------------------------------------------------
# The editor's per-form DV content record and the game's TechData both live at
# per-Digimon record offsets; these are the game's own field names.
TECH_FIELDS = {
    "mp": (0x00, "u16"),            # its cost
    "power": (0x02, "u16"),         # its damage
    "icon": (0x04, "u8"),           # a frame of the menu sprites
    "kind": (0x05, "u8"),           # 3: heals the target
    "accuracy": (0x06, "u8"),
    "element": (0x07, "u8"),        # ELEMENT_FIRST and up
    "elementPower": (0x08, "u8"),
    "family": (0x09, "u8"),         # FAMILY_FIRST and up
    "effect": (0x0A, "u8"),         # TECH_EFFECT_*
    "effectChance": (0x0B, "u8"),
    "effectPower": (0x0C, "u8"),
    "unkD": (0x0D, "u8"),
    "unkE": (0x0E, "u8"),
    "unkF": (0x0F, "u8"),
    "unk10": (0x10, "u8"),
    "hitCount": (0x11, "u8"),
}
TECH_KIND_HEAL = 3

# --------------------------------------------------------------------------
# Battle constants (fightstg.h)
# --------------------------------------------------------------------------
# PartnerStats.stats / PartnerTotals indices (game_state.h:222-227).
STAT_LEVEL = 0
STAT_HP = 2
STAT_MAX_HP = 3
STAT_MP = 4
STAT_MAX_MP = 5

# Battle stats (fightstg.h:250-254).
BATTLE_STAT_ATTACK = 0
BATTLE_STAT_DEFENSE = 1
BATTLE_STAT_SPIRIT = 2
BATTLE_STAT_WISDOM = 3
BATTLE_STAT_SPEED = 4

# Elements / families (fightstg.h:258-260): index 2 and up; under it, none.
ELEMENT_FIRST = 2
ELEMENT_COUNT = 7
FAMILY_FIRST = 2

# TechData.kind (fightstg.h:263-264).
TECH_PHYSICAL = 2
TECH_MAGIC = 3

# Fighter condition/status codes (fightstg.h:1346-1351). These are RUNTIME
# battle flags on BattleFighter.flags (fightstg.h:1368), set during a battle
# (e.g. fightstg_6.c and friends `fighter->flags |= FIGHTER_POISONED`); they
# live in the battle struct, NOT in GAME, so they are NOT saved and a save
# editor has no page for them. Kept here as names for battle-behaviour readers.
FIGHTER_POISONED = 1
FIGHTER_PARALYZED = 2
FIGHTER_CONFUSED = 4
FIGHTER_ASLEEP = 8
FIGHTER_NO_SWITCH = 0x10
FIGHTER_NO_DIGIVOLVE = 0x20
FIGHTER_STATUS_RUNTIME_ONLY = True  # not part of the save (see note above)

# Technique effects (fightstg.h:1403-1423); TECH_EFFECT_FIRST and up, under it
# the technique has none. 2-35.
TECH_EFFECT_FIRST = 2
TECH_EFFECT_POISON = 2
TECH_EFFECT_PARALYSIS = 3
TECH_EFFECT_CONFUSION = 4
TECH_EFFECT_SLEEP = 5
TECH_EFFECT_KNOCK_OUT = 6
TECH_EFFECT_DRAIN = 8               # HP, in 128ths of the damage
TECH_EFFECT_MULTI_HIT = 9
TECH_EFFECT_ENEMY_ONLY = 10
TECH_EFFECT_CRITICAL = 11
TECH_EFFECT_STEAL = 12
TECH_EFFECT_NO_SWITCH = 13
TECH_EFFECT_NO_DIGIVOLVE = 14
TECH_EFFECT_LOWER_ATTACK = 26
TECH_EFFECT_LOWER_DEFENSE = 27
TECH_EFFECT_DRAIN_MP = 29
TECH_EFFECT_DOUBLE_MAGIC = 31
TECH_EFFECT_RAISE_ONE_STATUS = 32
TECH_EFFECT_RAISE_EACH_STATUS = 33
TECH_EFFECT_RAISE_ALL_STATUS = 34
TECH_EFFECT_END_BATTLE = 35

# --------------------------------------------------------------------------
# Battle-event codes (fightstg.h:1260-1261) — for completeness of the tie-in.
# --------------------------------------------------------------------------
EVENT_STATUS_DAMAGE = 9
EVENT_STATUS_END = 10

# --------------------------------------------------------------------------
# Partner / PartnerStats beyond what the editor models (game_state.h:236-247)
# --------------------------------------------------------------------------
# The editor's per-Digimon record base is PartnerStats - 0x20 (== Partner - 0x14,
# save.py DIGI_STAT_BASE / DIGI_STAT_BASE_REL), so an editor-record offset is a
# PartnerStats offset + 0x20. These three fields are NOT modelled by the editor
# yet; the offsets below say where they would be if it ever exposes them.
PARTNER_STATS_EXTRA_FIELDS = {
    # name: (PartnerStats off, editor-record off, type)
    "status":     (0x042, 0x062, "s16[3]"),
    "slots":      (0x048, 0x068, "s16[4]"),
    "equip":      (0x3C0, 0x3E0, "s16[6]"),
}
# PARTNER_SLOT_COUNT (game_state.h:184): only the first three slots are used.
PARTNER_SLOT_COUNT = 3
PARTNER_ENTRY_COUNT = 44
FIRST_ENTRY_ID = 3          # game_state.h:187
PARTNER_COUNT = 8

# (1) status[3] — written by the raise-status techniques and cleared at an inn;
# no reader of it was found in the decomp, so its game-visible effect is NOT
# established (report it as ambiguous). Citations:
#   fightstg_6.c:6764-6772 FIGHTSTG_raiseOneStatus:  stats->status[i] += effectPower
#   fightstg_6.c:6776-6788 FIGHTSTG_raiseEachStatus: stats->status[i] += effectPower
#   fightstg_6.c:6792-6803 FIGHTSTG_raiseAllStatus:  stats->status[i] += effectPower
#   main/menu/inn.c:69     healParty: p->status[j] = 0  (rest restores it)
# TECH_EFFECT_RAISE_ONE/EACH/ALL_STATUS are 32/33/34 (fightstg.h:1420-1422).
STATUS_FIRST_EFFECT = TECH_EFFECT_RAISE_ONE_STATUS

# (2) slots[4] — the partner's battle team: the entry ids of up to
# PARTNER_SLOT_COUNT=3 Digimon it takes to battle. getPartnerSlots copies them
# and counts only slots[i] >= FIRST_ENTRY_ID (3) whose id is in entries[];
# setPartnerSlots stores the matching entry id, else -1. Valid values: -1
# (empty) or an id (>= 3) present in PartnerStats.entries[]. Citations:
#   main/game/partner.c:19-37 getPartnerSlots  (the ids it takes to battle)
#   main/game/partner.c:40-52 setPartnerSlots  (-1 when it does not have it)
#   stgdglab/stgdglab_2.c:297-322 the lab's second screen calls setPartnerSlots
#   stgdglab/stgdglab.h:6-7       "the second sets a partner's three slots"
# Partner.battleDigivolve (game_state.h:253) is the form the BATTLE STARTS the
# partner as; the lab picks it FROM these slots (stgdglab_2.c:378-385: one of
# slots[0..2], or 0 for the partner's own form), so the two are edited together.
BATTLE_DIGIVOLVE_OWN = 0    # 0 = fight as the partner's own form
PARTNER_SLOTS_SENTINEL = -1  # an empty battle slot

# (3) equip[6] — the per-partner equipment, the six item-id slots computeStats
# adds to the stats. Confirmed readers/citations:
#   main/game/stats.c:59-146 computeStats loops equip[0..5] (built from d->equip)
#   main/game/events.c:434-452 unequipItem (a kind-7 weapon clears [2] and [3])
#   stitshop/stitshop.c:2515-2681 the shop's equip/unequip UI (STITSHOP_equip)
# How an item maps to a slot is chosen by its ItemData kind (stitshop.c
# STITSHOP_compareEquip, :2523-2612: weapon kinds 4/5 -> equip[0]/equip[1];
# weapon kinds 1/2/3/7 -> equip[2]/equip[3], kind 7 filling both; accessory
# kinds 6/8 -> equip[4]/equip[5]). The four items of a set bonus are equip[0..3]
# (game_state.h:211); EQUIP_SETS/EQUIP_SET_BONUSES (stats.c:151-160).
EQUIP_SLOT_COUNT = 6
EQUIP_SET_SIZE = 4          # the four items a set bonus compares (equip[0..3])

# There IS a separate array: GameState.equippedItems (game_state.h:384,
# GAME offset 0x020F, s8[0x193] = a per-item COUNT of how many are equipped
# across the party). It is maintained alongside the bag: the game treats an item
# as owned when items[x] || equippedItems[x] (events.c:339,343) and equipping
# moves a count from items to equippedItems (stitshop.c:2670-2671
# ``GAME.equippedItems[id]++; GAME.items[id]--;``; unequipping does the
# reverse, :2626). So a save editor that changes a partner's equipment must keep
# GAME.equippedItems in sync with the bag counts (the editor already documents
# this trap: save.py's equippedItems note).
EQUIPPED_ITEMS_OFF = 0x020F     # == save.GAME_FIELDS["equippedItems"]

# --------------------------------------------------------------------------
# Memory-card constants and field names (include/dw3/memcard.h,
# include/stgmcard.h). Values are unchanged and match save.py's model.
# --------------------------------------------------------------------------
CARD_SECTOR_SIZE = 0x80     # memcard.h:12 the bytes read/written at once
SAVE_BLOCKS = 4             # memcard.h:15
SAVE_MAX_ICONS = 3          # memcard.h:18
CARD_HEADER_ICONS = 0x10    # memcard.h:21 CardHeader.type flag for icons
CARD_MAX_FILES = 15         # memcard.h:24 files listed from a card's directory

# MemCard.result (memcard.h:30-34), as libmcrd's MemCardSync reports it.
CARD_ERR_NONE = 0
CARD_ERR_NO_CARD = 1
CARD_ERR_NEW_CARD = 3
CARD_ERR_UNFORMATTED = 4
CARD_ERR_NOT_STARTED = 8

# MemCard.state (memcard.h:37-44).
MEMCARD_IDLE = 0
MEMCARD_CHECKING = 1
MEMCARD_ACCEPTING = 2
MEMCARD_READING = 3
MEMCARD_WRITING = 4
MEMCARD_COMMAND = 5
MEMCARD_STATES = ("MEMCARD_IDLE", "MEMCARD_CHECKING", "MEMCARD_ACCEPTING",
                  "MEMCARD_READING", "MEMCARD_WRITING", "MEMCARD_COMMAND")

# readSave/writeSave sections (memcard.h:47).
SAVE_SECTION_HEADER = 0
SAVE_SECTION_INFO = 1
SAVE_SECTION_DATA = 2
SAVE_SECTIONS = ("SAVE_SECTION_HEADER", "SAVE_SECTION_INFO", "SAVE_SECTION_DATA")

# memCardCommand's operations (memcard.h:50).
MEMCARD_OP_LIST = 0
MEMCARD_OP_CREATE = 1
MEMCARD_OP_FORMAT = 2
MEMCARD_OP_UNFORMAT = 3
MEMCARD_OPS = ("MEMCARD_OP_LIST", "MEMCARD_OP_CREATE", "MEMCARD_OP_FORMAT",
               "MEMCARD_OP_UNFORMAT")

# MemCardSave fields (stgmcard.h:80-90). +0x18 was long called unk18 and
# +0x1C unk1C; upstream names them area and place.
MEMCARD_SAVE_FIELDS = {
    "name": (0x00, "u8[0x18]"),      # empty for a free slot
    "area": (0x18, "s32"),           # a string of TEXT_AREA_NAMES
    "place": (0x1C, "s32"),          # a string of TEXT_SHOP_NAMES
    "money": (0x20, "s32"),
    "time": (0x24, "PlayTime"),      # frames, hours, minutes, seconds, maxed
    "partners": (0x30, "s32[3]"),    # 3 and up: a partner
    "levels": (0x3C, "s16[3]"),
    "unk42": (0x42, "s16"),
}
assert MEMCARD_SAVE_FIELDS["area"][0] == _save.F_AREA == 0x18
assert MEMCARD_SAVE_FIELDS["place"][0] == _save.F_SHOP == 0x1C

MEMCARD_SAVE_VERSION_BY_VERSION = {3: 3, 4: 4}   # MEMCARD_SAVE_VERSION (== version byte)
MEMCARD_FILE_MAGIC = 0x33574D44                  # stgmcard.h:103 "DMW3"


__all__ = [
    # constants/tables
    "DATA_SECTION_BASE", "GAME_FIELDS", "GAME_SAVE_SIZE_BY_VERSION",
    "MODE_OVERLAY", "PROGRESS_PAYLOAD", "PARTY_SET_PAYLOAD",
    "FLAG_GROUPS", "flag_group_payload_offset", "flag_group_size",
    "FLAG_GROUPS_PAYLOAD_SPAN", "SPECIAL_CONDITIONS", "PROGRESS_RANGES",
    "PARTY_STAT_THRESHOLDS", "MONEY_REQUIRED", "MONEY_GAINS", "MONEY_LOSSES",
    "TECH_FIELDS", "MEMCARD_SAVE_FIELDS", "PARTNER_STATS_EXTRA_FIELDS",
    "EQUIPPED_ITEMS_OFF",
    # code builders
    "FLAG", "PROGRESS", "SPECIAL", "PARTY_STAT", "EVENT_BATTLE", "CARD_BATTLE",
    "WARP_ARG", "ITEM", "START_EVENT", "CARD", "CODES_END",
    "event_group", "action_group", "event_code_id",
]
