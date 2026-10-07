"""DMW3 save-record model.

Every offset here is CONFIRMED against both sample cards and cross-checked
against the ddw3 decompilation. See docs/SAVE_FORMAT.md for the evidence.

Design rule: this module refuses to guess. Fields we have not proven are simply
absent rather than exposed with invented meanings.

The game's OWN names for the bytes this module reads (flags, progress, modes,
techniques, battle codes, memory-card constants) live in
:mod:`dmw3editor.core.game_state`, copied from the upstream decompilation
(ReGame-Labs/dw3_decomp). That module is documentation-only; this one is the
executable model.
"""

from __future__ import annotations

import json
import pathlib
import struct
from dataclasses import dataclass, field

from . import checksum as _ck
from dmw3editor.paths import pkg_dir

DATA_DIR = pkg_dir() / "data"

PAYLOAD_SIZE = 32768
TITLE_FRAME_END = 0x0200

# ---- the DMW3 card file (decomp: include/stgmcard.h, memcard.c) -----------
# The save file is 4 blocks (32768 bytes):
#   0x0000..0x0200  PS1 title frame: 'SC' + 3 icon frames (128 bytes each)
#   0x0200..0x0300  info section: MemCardFile (0xD4) + 0x2C zero padding
#   0x0300..0x2A00  data section 0 (one GameSave; stride 0x2700)
#   0x2A00..0x5100  data section 1
#   0x5100..0x7800  data section 2
#   0x7800..0x8000  trailing 0x800 bytes (not written by the game; unidentified)
RECORD_BASE = 0x0200            # MemCardFile, payload-absolute
MAGIC_OFFSET = 0x0204           # s32 "DMW3"
MAGIC = b"DMW3"
VERSION_OFFSET = 0x0202         # u8: 3 = USA, 4 = EUR (MEMCARD_SAVE_VERSION)
H_CHECKSUM = 0x0200             # u8: XOR8 over [0x0204, 0x02D4)
H_LAST = 0x0201                 # u8: the slot last saved to (0..2)
H_VERSION = VERSION_OFFSET      # u8
H_UNK3 = 0x0203                 # u8: unidentified (0 on both region cards)
H_MAGIC = MAGIC_OFFSET          # s32
INFO_SECTION_SIZE = 0x100       # MEMCARD.infoSize (system.c initMemCard)
DATA_SECTION_OFFSETS = (0x0300, 0x2A00, 0x5100)  # data sections (dataSize stride)
DATA_SECTION_SIZE = 0x2700      # MEMCARD.dataSize
GAME_SAVE_SIZE_BY_VERSION = {3: 0x26BC, 4: 0x26C4}  # GAME_SAVE_SIZE per region

# Card collection: 314 byte-per-card counts (0-9) at payload 0x06A3.
# CONFIRMED 2026-09-02: the user's card has exactly 314 consecutive 0x09 bytes
# there; setting index 0 to 8 made the game show "Sacred Spear: 8" (research
# card table index 0 == Sacred Spear == card #1). Index N == card number N+1.
CARD_BASE = 0x06A3
CARD_COUNT = 314
CARD_MAX = 9

# Item quantity array: byte-per-item at payload 0x03A7.
# CONFIRMED 2026-09-02 via controlled diff + live item-screen anchors.
#
# WHERE THE BASE COMES FROM (upstream reconciliation, 2026-10-07). GAME.items
# (game_state.h:383) sits at payload 0x037C, and 0x03A7 = 0x037C + 0x2B: the
# editor's item array is indexed into the SAME array GAME.items is, offset by
# 0x2B (43) so that slot 0 is item id 0x2B — the FIRST USABLE ITEM. The decomp's
# item-id lists (src/main/game/items.c:338-349) put KEY_ITEM_IDS at 1..0x2A and
# start USABLE_ITEM_IDS at 0x2B, so 0x2B is exactly "first usable item"
# ("Power Charge"). GAME.items[] is indexed by item id (1-based ITEMS[id-1]),
# as the key-item block below independently shows (items[4..0x2A] == key-item
# ids 4..42). The older note here ("ItemId enum idx - 35" / "[decomp 35-83]")
# undercounts by 8; the real delta is 0x2B.
# Layout (save slot + 0x2B == decomp item id):
#   items       slots 0-48   (49: Power Charge -> Booster 01a)   [decomp 0x2B-0x5B]
#   weapons     slots 49-171 (123: Short Sword -> Glorious Horn) [decomp 0x5C-0xD6]
#   armor       slots 172-248 (77)                               [decomp 0xD7-0x123]
#   accessories slots 249-316 (68)                               [decomp 0x124-0x167]
#   card packs  slots 317-351 (35: Monmon DDNA, Booster 02a-15b,
#               R-Booster 01-05)                                 [decomp 0x168-0x18A]
# NOTE: the earlier "important items at 317-351" label was WRONG (withdrawn
# 2026-09-02). Those slots are booster card packs (player-verified via the
# item tab read-back). Key items live in their own flag arrays, below.
ITEM_BASE = 0x03A7
ITEM_COUNT = 352          # total slots in the save's item array
ITEM_MAX = 99
ITEM_SUBCLASS_BOUNDS = (  # (start_slot, end_exclusive, label)
    (0, 49, "Items"),
    (49, 172, "Weapons"),
    (172, 249, "Armor"),
    (249, 317, "Accessories"),
    (317, 352, "Card Packs"),
)

# Key/important item flags (byte = 1 when owned, 0 otherwise).
# CONFIRMED 2026-09-02 against the player's Important screen (35 items read
# top-to-bottom, byte-exact).
#   Block A: payload 0x0380..0x03A6 (39 flags) == usitmnam display idx 4..42
#     Tree Boots -> Kotemon DDNA (idx 4-19), Sun Trophy (20, unowned in EUR),
#     Sepik Mask -> Black ID Pass (21-25), 8 Tags (26-33, unowned),
#     Guilmon/Veemon DDNA (34-35), World Champ + Asuka Medal (36-37, unowned),
#     Renamon DDNA -> Platinum ID (38-42).
#   Monmon DDNA is stored as main-array slot 317 (payload 0x04E4) but shows in
#   the Important screen between Platinum ID and Kumamon DDNA.
#   Block B: payload 0x0507..0x050E (8 flags) == usitmnam display idx 395..402
#     Kumamon DDNA, Crony ID, Etemon's Mike (unowned in EUR), Blue Card,
#     8lue Card (a separate item - game typo), Recovery CD 3 (unowned),
#     Staff Pass, Folder Bag.
KEY_ITEMS_BLOCK_A = (0x0380, 0x03A7)   # 39 bytes
KEY_ITEMS_BLOCK_B = (0x0507, 0x050F)   # 8 bytes
KEY_ITEM_MAX = 1

# Key items stored in main item array (slot -> name). Currently only Monmon
# DDNA at slot 317 (payload 0x04E4); those show in the Important screen even
# though their byte lives in the item-qty array. When adding entries here,
# keep slot numbers within ITEM_COUNT.
KEY_ITEM_MAIN_SLOTS = {317: "Monmon DDNA"}

# 35 card packs: Booster 01a is item-array slot 48; Booster 02a-15a slots
# 318-331; Booster 1b-15b slots 332-346; R-Booster 01-05 slots 347-351.
# Names and slots come from dmw3editor/data/packs_ids.json (usitmnam order).
PACK_COUNT = 35

# Per-Digimon current-stat blocks (CONFIRMED 2026-09-02 via live HP/MP/stat/
# level/EXP anchor tests on the player's maxed EUR card).
# Records at payload 0x0A48, stride 0x3DC, roster_index 0-7 =
# Kotemon, Kumamon, Monmon, Agumon, Veemon, Guilmon, Renamon, Patamon
# (decomp DigimonId 1-8, confirmed by unique-HP read-back).
#
# WHY THE BASE IS 0x14 BEFORE GAME.partners (upstream reconciliation,
# 2026-10-07). GAME.partners (game_state.h:389) sits at payload 0x0A5C and each
# Partner is 0x3DC bytes — exactly DIGI_STAT_STRIDE. The editor anchors its
# record 0x14 bytes before Partner[i], but every field offset is calibrated to
# land on the decomp's fields, so the ABSOLUTE addresses are identical:
#   D_EXP   0x38 -> Partner+0x24 == PartnerStats.exp     (game_state.h:238)
#   D_LEVEL 0x3C -> Partner+0x28 == PartnerStats.stats[0] (STAT_LEVEL)
#   D_HP    0x40 -> Partner+0x2C == stats[2] (STAT_HP);  D_MP 0x44 -> stats[4]
#   D_STATS 0x48 -> Partner+0x34 == stats[6] (the 6 battle stats + 7 resistances)
#   D_UNLOCK 0x18 -> Partner+0x04 == Partner.unlocked
# The 0x14 is only the anchor's padding; no field moves. See
# dmw3editor/core/game_state.py for the full named map.
DIGI_STAT_BASE = 0x0A48
DIGI_STAT_STRIDE = 0x3DC
DIGI_STAT_COUNT = 8
# Field offsets relative to a record base.
D_EXP = 0x38     # u32
D_LEVEL = 0x3C   # u16
D_MAX_LEVEL = 0x3E  # u16 (99)
D_HP = 0x40      # u16 current
D_HP_MAX = 0x42  # u16 max
D_MP = 0x44      # u16 current
D_MP_MAX = 0x46  # u16 max
# 13 base stats (u16 each), display order confirmed by the player:
# Strength, Defense, Spirit, Wisdom, Speed, Charisma, Fire, Water, Ice,
# Wind, Lightning, Machine, Dark.
D_STATS = 0x48
DIGI_STAT_NAMES = (
    "Strength", "Defense", "Spirit", "Wisdom", "Speed", "Charisma",
    "Fire", "Water", "Ice", "Wind", "Lightning", "Machine", "Dark",
)
DIGI_ROSTER_NAMES = (
    "Kotemon", "Kumamon", "Monmon", "Agumon",
    "Veemon", "Guilmon", "Renamon", "Patamon",
)
DIGI_MAX_LEVEL = 99
DIGI_MAX_EXP = 999999
DIGI_MAX_HP = 9999
DIGI_MAX_MP = 9999
DIGI_MAX_STAT = 999

# Per-digimon DIGIVOLUTION DV levels (CONFIRMED 2026-09-02 via live probe).
# Each roster record (0x0A48 + idx*0x3DC) stores its digivolution forms'
# DV levels as u16s. Layout (verified by user's in-game DV screen read-back:
# writing slot+18..19 to 0x51..0x5A showed 81..90 on the matching forms in
# exact slot order):
#   header form (the "row 1" digivolution, e.g. Greymon for Agumon): u16 at
#     record+0x72
#   data slots: 43 slots at record+0x74 + 20*k (k=0..42); each slot is:
#     +0..+3   slot prefix (constant per record region)
#     +4..+15  per-form extra data (0 for earned-but-never-used forms)
#     +16..17  FORM MARKER u16 — constant per evolved form across cards
#     +18..19  DV level u16 (0 = not earned)
# Level 0 marks a slot as not-yet-kept, BUT the level field alone does NOT
# decide whether the game shows the form: probe 2 (all 43 slots of all 8
# digimon tagged 1..43) left unearned forms hidden. The form marker at
# +16..17 is what registers a form in a slot — probe CONFIRMED 2026-09-02 by
# the user: writing Seraphimon's marker + level 42 into an empty Agumon slot
# made "Seraphimon 42" appear on the in-game DV screen (same for Rosemon 7).
D_DV_HEADER = 0x72        # u16: header/primary digivolution level
D_DV_SLOTS = 0x74         # 43 slots * 20 bytes, each slot's level at +18
D_DV_SLOT_STRIDE = 20
D_DV_SLOT_CONTENT = 4     # 12-byte per-form tech record inside a slot (+4..+15)
D_DV_SLOT_CONTENT_LEN = 12
D_DV_SLOT_LEVEL = 18
D_DV_SLOT_MARKER = 16     # u16: per-form identity marker (0 = not earned)
D_DV_COUNT = 43
D_DV_MAX = 99

# PROBE-CONFIRMED 2026-09-03: the DV screen shows a slot's techniques from the
# CONTENT FIELD OF THE *NEXT* SLOT (record + D_DV_SLOTS + (k+1)*20 + 4). Natural
# cards: content[k+1] = profile techs of the form at slot k, scaled by that
# form's DV level (learn thresholds). Zero content => the row shows no moves.
# V5 probe: filling content[31..42] made every force-earned row 30..41 show its
# full tech list in-game; V3 probe: zeroing slot 31's content removed Devimon's
# (slot 30) techs. The header (row-1) form's own techs live in content[0]; slot
# 42's display record would be a 43rd slot whose 12 content bytes physically
# spill past the record end into the next record's first 4 bytes (natural USA
# card ri5 does exactly this for Sakuyamon at slot 42).
D_DV_SLOT_CONTENT_PHANTOM = 0x3D4  # record-relative: display content for slot 42

# Constant per-form identity markers (u16 @ slot+16..17). Measured 2026-09-02
# from the EUR maxed card (43 forms; every named slot matched) plus the USA
# genuine card (Diaboromon = 151, which EUR never earned). Card-independent:
# the same form carries the same marker wherever it appears.
DV_FORM_MARKERS = {
    "Angemon": 20, "Angewomon": 234, "Armormon": 390, "BKWarGreymon": 267,
    "Beelzemon": 377, "Cannondramon": 393, "Devimon": 6, "Diaboromon": 151,
    "Digitamamon": 56, "Dinohumon": 386, "ExVeemon": 259, "Gallantmon": 369,
    "GranKuwagamon": 230, "GrapLeomon": 391, "Greymon": 5, "Grizzmon": 388,
    "Growlmon": 367, "GuardiAngemon": 392, "Hookmon": 387,
    "Imperialdramon": 148, "ImperialdramonFM": 359, "ImperialdramonPM": 381,
    "Kabuterimon": 19, "Kyubimon": 374, "Kyukimon": 389, "MagnaAngemon": 211,
    "MaloMyotismon": 378, "Marsmon": 394, "MegaGargomon": 372,
    "MetalGarurumon": 196, "MetalGreymon": 12, "MetalMamemon": 27,
    "Myotismon": 66, "Omnimon": 150, "Paildramon": 254, "Phoenixmon": 59,
    "Rosemon": 144, "Sakuyamon": 376, "Seraphimon": 214, "SkullGreymon": 26,
    "Stingmon": 260, "Taomon": 375, "WarGreymon": 213, "WarGrowlmon": 368,
}

D_DV_HEADER_FORMS = {  # header (row-1) digivolution per roster partner
    "Kotemon": "Dinohumon", "Kumamon": "Grizzmon", "Monmon": "Hookmon",
    "Agumon": "Greymon", "Veemon": "ExVeemon", "Guilmon": "Growlmon",
    "Renamon": "Kyubimon", "Patamon": "Angemon",
}

DV_FORM_MARKER_NAMES = {  # reverse of DV_FORM_MARKERS (marker -> form name)
    v: k for k, v in DV_FORM_MARKERS.items()
}

# PARTY/partner-ID space — REGION-INDEPENDENT (corrected 2026-10-05).
# The save record's partner fields hold the decomp's Partner.unlocked value:
# partner_index + 3, where 0 = empty/locked. CONFIRMED against the
# decompilation of BOTH releases (the code is shared, not #if-gated):
#   stgmcard.c:1106  save->partners[i] = dataBuf->partners[member].unlocked;
#   game_state.h:193  s32 unlocked; /* partner id + 3, 0 while locked */
#   stgmcard.c:200/293/305  the save-list UI indexes its partner
#                    sprites/animations by (partners[i] - 3)
# So a live party member is id 3..10 == Kotemon..Patamon on the USA AND the
# EUR card alike. The USA/EUR save+load path is identical here — only the
# MEMCARD_SAVE_VERSION byte (3 vs 4) differs between the regions.
# EVIDENCE: both live cards (Builds/USA/card1.mcd, Builds/EUR/card1.mcd) hold
# the same raw partners[3] = [4, 8, 10] -> Kumamon, Guilmon, Patamon. The
# previous "USA = 1..8" guess decoded that same card as Agumon/Patamon/
# Invalid #10. 0 = empty; evolved forms are battle-DV mechanics, never stored
# in these fields.
PARTY_MIN_BY_REGION = {"USA": 3, "EUR": 3}
PARTY_ROOKIE_IDS = (3, 4, 5, 6, 7, 8, 9, 10)   # names == DIGI_ROSTER_NAMES


def party_min_for_region(region: str) -> int:
    """First valid party-space id (3). The space is region-INDEPENDENT.

    Kept as a region-keyed lookup so the id space stays in one place, but the
    decomp shows USA and EUR store Partner.unlocked = index + 3 identically.
    """
    return PARTY_MIN_BY_REGION.get(region, PARTY_MIN_BY_REGION["USA"])


def party_index_for_id(digimon_id: int, region: str) -> int:
    """Data-section partner INDEX for a summary party id (region-aware).

    The info summary stores Partner.unlocked (id = index + party_min) but the
    data section stores the index itself, so the two differ by the region's
    party minimum. MEASURED: the game-written ``unlocked`` fields on both live
    cards are Kumamon=4, Guilmon=8, Patamon=10 -- index + 3 on the USA card AND
    the EUR card alike, matching game3.c:766 setParty (unlocked = index + 3).
    """
    return digimon_id - party_min_for_region(region)


def party_id_for_index(index: int, region: str) -> int:
    """Summary party id (Partner.unlocked) for a data-section partner index."""
    return index + party_min_for_region(region)

KEY_ITEM_COUNT = (
    KEY_ITEMS_BLOCK_A[1] - KEY_ITEMS_BLOCK_A[0]
    + KEY_ITEMS_BLOCK_B[1] - KEY_ITEMS_BLOCK_B[0]
    + len(KEY_ITEM_MAIN_SLOTS)
)  # 39 + 8 + 1 = 48 flags in Important-screen order


def key_item_offset(key_index: int) -> int:
    """Payload offset of the key-item flag for an Important-screen index.

    Order mirrors the player's in-game Important screen:
      0..38   = Block A (Tree Boots ... Platinum ID)   [0x0380..0x03A6]
      39      = Monmon DDNA (main array slot 317)      [0x04E4]
      40..47  = Block B (Kumamon DDNA ... Folder Bag)  [0x0507..0x050E]
    """
    a_start, a_end = KEY_ITEMS_BLOCK_A
    n_a = a_end - a_start
    if key_index < n_a:
        return a_start + key_index
    key_index -= n_a
    if key_index == 0:  # Monmon DDNA
        return ITEM_BASE + 317
    key_index -= 1
    b_start, b_end = KEY_ITEMS_BLOCK_B
    if key_index < b_end - b_start:
        return b_start + key_index
    raise SaveError(f"key item index out of range: {key_index}")

# The three per-save summaries (decomp: MemCardFile saves[3]).
SLOT_OFFSETS = (0x0208, 0x024C, 0x0290)
SLOT_SIZE = 0x44

# Field offsets relative to a slot's start (decomp: MemCardSave, 0x44 bytes).
F_NAME = 0x00            # u8[0x18]: C string; [0] == 0 marks a free slot
F_NAME_SIZE = 0x18
F_AREA = 0x18            # s32: MemCardSave.area — index into TEXT_AREA_NAMES
F_PLACE = 0x1C           # s32: MemCardSave.place — index into TEXT_SHOP_NAMES
# Upstream (stgmcard.h:82-83) names +0x18 ``area`` and +0x1C ``place``; both are
# drawn as text by the save list (TEXT_AREA_NAMES[TEXT_SHOP_NAMES]). This offset
# was long called F_SHOP / "shop" here — the game calls it PLACE (the save point
# shown, named after the shop list). ``F_SHOP`` is kept as the deprecated alias.
F_MONEY = 0x20           # s32: Bits
F_TIME_FRAMES = 0x24     # s32: play-time frames (PlayTime.frames)
F_HOURS = 0x28           # s16
F_MINUTES = 0x2A         # s16
F_SECONDS = 0x2C         # s16
F_TIME_MAXED = 0x2E      # s16: PlayTime.maxed
F_PARTY_IDS = (0x30, 0x34, 0x38)     # s32 each: Partner.unlocked (index + 3)
F_PARTY_LEVELS = (0x3C, 0x3E, 0x40)  # s16 each
F_UNK42 = 0x42           # s16: unidentified

# DEPRECATED aliases kept for the shipped API and its tests. 0x18 was long read
# as a Digimon "partner"; the decomp (stgmcard.c:167) proves it is the save's
# AREA index. 0x1C was long read as "shop"; upstream (stgmcard.h:83) names it
# PLACE. The real party ids are F_PARTY_IDS — there is no single partner field
# in a MemCardSave.
F_PARTNER = F_AREA
F_SHOP = F_PLACE
F_UNKNOWN_1C = F_PLACE

MONEY_MAX = 9_999_999
LEVEL_MAX = 99
LEVEL_MIN = 1
HOURS_MAX = 999
PARTY_SIZE = 3

# ---- the AUTHORITATIVE running party lives in the DATA SECTION ------------
# The game does not run the party from the info-summary partners[3]; it copies
# the whole GameSave out of the data section on load:
#   stgmcard.c:1027  *(GameSave *)&GAME = *(GameSave *)STGMCard_funcs.dataBuf;
#   game_state.h:297  /* 0x0070 */ s32 party[3]; /* partner indices */
# so GameState.party holds partner INDICES (0..7), whereas the summary holds
# Partner.unlocked = index + 3 (stgmcard.c:1106; game_state.h:193). A party
# swap must therefore write BOTH, converting id <-> index with the region's
# party minimum. Writing only the summary changes what the card list shows but
# leaves the loaded party untouched -- the reported "swap did not take effect".
GS_PARTY = 0x0070                      # GameState.party[3] within a data section
DIGI_STAT_BASE_REL = DIGI_STAT_BASE - DATA_SECTION_OFFSETS[0]  # 0x0748 (Partner[0]-0x14)
# Partner.unlocked (game_state.h:193 /* 0x004 */ s32 unlocked; partner id + 3,
# 0 while locked), relative to the DIGI_STAT_BASE convention above.
D_UNLOCK = 0x18                        # == Partner+0x04

# Regions unidentified; writing there is refused.
FORBIDDEN_REGIONS = ((0x2900, 0x5000), (0x5000, 0x7700))


def _load_ids(name: str) -> dict[int, str]:
    path = DATA_DIR / f"{name}_ids.json"
    if not path.exists():
        return {}
    raw = json.loads(path.read_text(encoding="utf-8"))
    return {int(k): v["name"] for k, v in raw["ids"].items()}


def _load_key_item_meta() -> dict[int, dict]:
    """Full key-item rows: name + eur_name + jp_in_eur flag.

    The EUR English table (esitmnam.toml) left the 8 card-battle Tags and
    Recovery CD 3 as untranslated Japanese strings, and uses Koc Trophy /
    Platinum Card where the USA table says World Champ / Asuka Medal.
    CONFIRMED 2026-09-02 from the decompiled lang files.
    """
    path = DATA_DIR / "key_items_ids.json"
    if not path.exists():
        return {}
    raw = json.loads(path.read_text(encoding="utf-8"))
    return {int(k): v for k, v in raw["ids"].items()}


def _load_pack_slots() -> dict[int, int]:
    """pack index -> save item-array slot, from packs_ids.json."""
    path = DATA_DIR / "packs_ids.json"
    if not path.exists():
        return {}
    raw = json.loads(path.read_text(encoding="utf-8"))
    return {int(k): int(v["save_slot"]) for k, v in raw["ids"].items()}


def _load_dv_orders() -> dict[str, dict]:
    """Per-roster digimon digivolution orders (header + earned slot list).

    digivolve_orders.json is keyed by display name (Kotemon..Patamon); each
    value is {"header": <primary form name>, "slots": [earned forms in slot
    order]}. CONFIRMED 2026-09-02 against the user's in-game DV screen reads
    (slot level tags 1..43 displayed forms in exact list order).
    """
    path = DATA_DIR / "digivolve_orders.json"
    if not path.exists():
        return {}
    return json.loads(path.read_text(encoding="utf-8"))


def _load_dv_content_donors() -> dict[int, bytes]:
    """marker -> 12-byte canonical technique content observed on natural cards.

    dv_content_donors.json was harvested (2026-09-03) from natural EUR/USA
    lv99 slots: for each form, the content field of the FOLLOWING slot, which
    the DV screen reads as that form's technique list. These are the exact
    bytes the game itself writes; fallback synthesis uses the profile techs.
    """
    path = DATA_DIR / "dv_content_donors.json"
    if not path.exists():
        return {}
    return {int(k): bytes.fromhex(v) for k, v in json.loads(path.read_text(encoding="utf-8")).items()}


def _load_dv_tech_profiles() -> dict[int, dict]:
    """marker -> {"techs": [5 ints], "learn": [5 ints]} from digimon_profiles.toml.

    value0 in the decompiled profiles equals the DV slot marker. 'techs' are
    the five technique ids and 'learn' the DV level at which each is learned.
    """
    path = DATA_DIR / "dv_tech_profiles.json"
    if not path.exists():
        return {}
    return {int(k): v for k, v in json.loads(path.read_text(encoding="utf-8")).items()}


_DV_CONTENT_DONORS: dict[int, bytes] | None = None
_DV_TECH_PROFILES: dict[int, dict] | None = None


def _dv_content_donors() -> dict[int, bytes]:
    global _DV_CONTENT_DONORS
    if _DV_CONTENT_DONORS is None:
        _DV_CONTENT_DONORS = _load_dv_content_donors()
    return _DV_CONTENT_DONORS


def _dv_tech_profiles() -> dict[int, dict]:
    global _DV_TECH_PROFILES
    if _DV_TECH_PROFILES is None:
        _DV_TECH_PROFILES = _load_dv_tech_profiles()
    return _DV_TECH_PROFILES


def dv_tech_content(marker: int, level: int) -> bytes:
    """12-byte content record for a form at a DV level.

    The DV screen reads a slot's displayed techniques from the *following*
    slot's content field (probe-confirmed 2026-09-03). Natural cards store the
    form's own profile techs there, one u16-style pair (id, 0x20) per learned
    technique plus a per-record tail, with unlearned pairs zeroed. Content is
    seeded from the canonical lv99 donor harvested from natural cards, then
    zeroes any pair whose learn threshold exceeds ``level`` so a form force-
    earned at DV 20 shows exactly the techniques it would know at 20.
    """
    donor = _dv_content_donors().get(marker)
    if donor is None or len(donor) != 12:
        donor = bytes(12)
    prof = _dv_tech_profiles().get(marker, {})
    techs = prof.get("techs") or []
    learn = prof.get("learn") or []
    out = bytearray(donor)
    for i in range(min(5, len(techs))):
        if not techs[i]:
            continue
        need = learn[i] if i < len(learn) else 0
        if need and level < need:
            out[2 * i:2 * i + 2] = b"\x00\x00"
    return bytes(out)


class IdTables:
    """Canonical id<->name tables from the decompilation."""

    def __init__(self) -> None:
        self.digimon = _load_ids("digimon")
        self.item = _load_ids("item")
        self.enemy = _load_ids("enemy")
        self.card = _load_ids("card")
        self.key_item = _load_ids("key_items")
        self._key_item_meta = _load_key_item_meta()
        self.pack = _load_ids("packs")
        self._pack_slot = _load_pack_slots()
        self._dv_orders = _load_dv_orders()

    # -- digivolution orders ----------------------------------------------
    def dv_order(self, roster_name: str) -> tuple[str, list[str]]:
        """(header form name, earned slot forms in slot order) for a roster name."""
        row = self._dv_orders.get(roster_name)
        if not row:
            return "", []
        return row.get("header", ""), list(row.get("slots", []))

    def dv_form_name(self, roster_name: str, slot_index: int) -> str:
        """Earned form name for a data slot, or '' when not on this card."""
        _, slots = self.dv_order(roster_name)
        if 0 <= slot_index < len(slots):
            return slots[slot_index]
        return ""

    # -- key items --------------------------------------------------------
    def key_item_name(self, index: int, region: str = "USA") -> str:
        """Key/important item name in Important-screen order. index 0 == Tree Boots.

        Region-aware: EUR uses eur_name when the localization differs
        (Koc Trophy / Platinum Card) or was never translated (the 8 Tags +
        Recovery CD 3 display as Japanese in the EUR game).
        """
        row = self._key_item_meta.get(index)
        if row is None:
            return f"Key item #{index + 1}"
        if region == "EUR" and row.get("eur_name"):
            return row["eur_name"]
        return row.get("name", f"Key item #{index + 1}")

    def key_item_jp_in_eur(self, index: int) -> bool:
        """True if this item's EUR English table string is untranslated Japanese."""
        row = self._key_item_meta.get(index)
        return bool(row and row.get("jp_in_eur"))

    def key_item_regions_differ(self, index: int) -> bool:
        """True if EUR and USA display names differ for this key item."""
        row = self._key_item_meta.get(index)
        return bool(row and row.get("eur_name"))

    def digimon_name(self, value: int) -> str:
        return self.digimon.get(value, f"Unknown #{value}")

    def item_name(self, value: int) -> str:
        return self.item.get(value, f"Unknown #{value}")

    def card_name(self, index: int) -> str:
        """Card display name. index 0..313 == card #1..#314 (save order)."""
        return self.card.get(index, f"Card #{index + 1}")

    def pack_name(self, index: int) -> str:
        """Card pack display name. index 0 == Booster 01a, 34 == R-Booster 05."""
        return self.pack.get(index, f"Pack #{index + 1}")

    def pack_save_slot(self, index: int) -> int:
        """Item-array slot (0x03A7 + slot) that stores this pack's count."""
        return self._pack_slot.get(index, -1)

    def pack_choices(self) -> list[tuple[int, int, str]]:
        """(pack_index, save_slot, name) sorted by save slot (game order)."""
        return sorted(
            (i, self._pack_slot.get(i, -1), self.pack.get(i, f"Pack #{i + 1}"))
            for i in self.pack
        )

    def card_choices(self) -> list[tuple[int, str]]:
        return sorted(self.card.items())

    def digimon_choices(self) -> list[tuple[int, str]]:
        return sorted(self.digimon.items())

    def base_rookie_choices(self, region: str = "USA") -> list[tuple[int, str]]:
        """The 8 base-stage partner digimon for party/partner fields.

        Ids are 3..10 == Kotemon..Patamon (the decomp's Partner.unlocked =
        index + 3), the SAME on USA and EUR (stgmcard.c:1106; game_state.h:193).
        Confirmed by live in-game reads (6/7/8 -> Agumon/Veemon/Guilmon,
        3 -> Kotemon; 0..2 invalid) and by both live cards. Evolved/enemy ids
        stall the game on load.
        """
        pmin = party_min_for_region(region)
        return [(pmin + i, DIGI_ROSTER_NAMES[i]) for i in range(8)]

    def party_name(self, party_id: int, region: str = "USA") -> str:
        """Name for a region-specific party-space id. 0 = empty."""
        if party_id == 0:
            return "Empty"
        pmin = party_min_for_region(region)
        if pmin <= party_id < pmin + 8:
            return DIGI_ROSTER_NAMES[party_id - pmin]
        return f"Invalid #{party_id}"


TABLES = IdTables()


class SaveError(Exception):
    """Raised when a save is malformed or an edit would be unsafe."""


@dataclass
class Slot:
    """One in-game save slot (68 bytes)."""

    index: int
    offset: int
    raw: bytes = field(repr=False)
    region: str = "USA"

    @property
    def is_empty(self) -> bool:
        return not any(self.raw)

    def _u32(self, rel: int) -> int:
        return struct.unpack_from("<I", self.raw, rel)[0]

    def _u16(self, rel: int) -> int:
        return struct.unpack_from("<H", self.raw, rel)[0]

    def _s16(self, rel: int) -> int:
        return struct.unpack_from("<h", self.raw, rel)[0]

    @property
    def area_id(self) -> int:
        """The save's AREA index (decomp stgmcard.c:167; MemCardSave.area, +0x18,
        a string of TEXT_AREA_NAMES)."""
        return self._u32(F_AREA)

    @property
    def place_id(self) -> int:
        """The save's PLACE index (decomp stgmcard.c:168; MemCardSave.place,
        +0x1C, a string of TEXT_SHOP_NAMES)."""
        return self._u32(F_PLACE)

    @property
    def shop_id(self) -> int:
        """DEPRECATED alias for :attr:`place_id` — upstream names +0x1C ``place``."""
        return self.place_id

    @property
    def partner_id(self) -> int:
        """DEPRECATED: this offset is the AREA index, not a Digimon.

        Kept because the legacy ``set_partner`` API and its tests use it; new
        code should use :attr:`area_id` and :attr:`party`.
        """
        return self.area_id

    @property
    def partner_name(self) -> str:
        """DEPRECATED: :attr:`area_id` decoded as a party-space id."""
        return TABLES.party_name(self.partner_id, self.region)

    @property
    def money(self) -> int:
        return self._u32(F_MONEY)

    @property
    def play_time(self) -> tuple[int, int, int]:
        return (self._u16(F_HOURS), self._u16(F_MINUTES), self._u16(F_SECONDS))

    @property
    def play_time_text(self) -> str:
        h, m, s = self.play_time
        return f"{h}:{m:02d}:{s:02d}"

    @property
    def play_frames(self) -> int:
        """PlayTime.frames — the game's 8.8 play counter (stgmcard.h)."""
        return self._u32(F_TIME_FRAMES)

    @property
    def play_time_maxed(self) -> int:
        """PlayTime.maxed (0 on both region cards)."""
        return self._s16(F_TIME_MAXED)

    @property
    def unk42(self) -> int:
        """MemCardSave.unk42 at slot +0x42; unidentified (0 on both cards)."""
        return self._s16(F_UNK42)

    @property
    def party(self) -> list[tuple[int, int]]:
        """[(digimon_id, level)] x3, positionally paired."""
        return [
            (self._u32(i), self._u16(lv))
            for i, lv in zip(F_PARTY_IDS, F_PARTY_LEVELS)
        ]

    @property
    def party_text(self) -> str:
        if self.is_empty:
            return "(empty slot)"
        return ", ".join(
            f"{TABLES.party_name(i, self.region)} Lv{lv}" for i, lv in self.party
        )

    @property
    def name_bytes(self) -> bytes:
        """The full 0x18-byte name field (decomp: MemCardSave.name[0x18])."""
        return self.raw[F_NAME:F_NAME + F_NAME_SIZE]

    @property
    def name_ascii(self) -> str:
        """Printable run in the name field, if any (EUR stores ASCII here)."""
        printable = bytes(c for c in self.name_bytes if 0x20 <= c < 0x7F)
        return printable.decode("ascii", "ignore")


class DMW3Save:
    """An editable DMW3 save payload (32,768 bytes)."""

    def __init__(self, payload: bytes | bytearray) -> None:
        if len(payload) != PAYLOAD_SIZE:
            raise SaveError(
                f"expected {PAYLOAD_SIZE}-byte payload, got {len(payload)}"
            )
        self._buf = bytearray(payload)
        if not self.has_magic:
            raise SaveError(
                "no 'DMW3' tag at 0x0204 - not a Digimon World 3 save"
            )
        self._region_override: str | None = None

    # ---- identity -------------------------------------------------------
    @property
    def has_magic(self) -> bool:
        return bytes(self._buf[MAGIC_OFFSET:MAGIC_OFFSET + 4]) == MAGIC

    @property
    def version(self) -> int:
        """MEMCARD_SAVE_VERSION byte at 0x0202 (3 = USA, 4 = EUR)."""
        return self._buf[VERSION_OFFSET]

    @property
    def last_saved_slot(self) -> int:
        """The slot the game last saved to (MemCardFile.last at 0x0201)."""
        return self._buf[H_LAST]

    @property
    def unk3(self) -> int:
        """MemCardFile.unk3 at 0x0203 (0 on both region cards)."""
        return self._buf[H_UNK3]

    @property
    def region_guess(self) -> str:
        if self._region_override:
            return self._region_override
        return {3: "USA", 4: "EUR"}.get(self.version, f"unknown (v{self.version})")

    def force_region(self, region: str) -> None:
        """Override the auto-detected region ("USA" or "EUR").

        Used by the ROM-region selector for cards whose version field is
        missing/unusual or to re-interpret an existing card. The payload
        itself is untouched — every page re-reads this via region_guess.
        """
        if region not in ("USA", "EUR"):
            raise SaveError(f"unsupported region override {region!r}")
        self._region_override = region

    def clear_region_override(self) -> None:
        """Drop a manual region override; region_guess auto-detects again."""
        self._region_override = None

    # ---- slots ----------------------------------------------------------
    @property
    def slots(self) -> list[Slot]:
        return [
            Slot(
                index=i,
                offset=off,
                raw=bytes(self._buf[off:off + SLOT_SIZE]),
                region=self.region_guess,
            )
            for i, off in enumerate(SLOT_OFFSETS)
        ]

    def slot(self, index: int) -> Slot:
        return self.slots[index]

    # ---- edits ----------------------------------------------------------
    def _guard(self, offset: int) -> None:
        for start, end in FORBIDDEN_REGIONS:
            if start <= offset < end:
                raise SaveError(
                    f"refusing to write 0x{offset:04X}: region "
                    f"0x{start:04X}-0x{end:04X} is not understood"
                )

    def _is_forbidden(self, offset: int, size: int = 1) -> bool:
        """True when [offset, offset+size) overlaps a refused region."""
        end = offset + size
        return any(start < end and offset < stop for start, stop in FORBIDDEN_REGIONS)

    def _write(self, offset: int, data: bytes) -> None:
        self._guard(offset)
        self._buf[offset:offset + len(data)] = data

    def _slot_base(self, slot_index: int) -> int:
        """Validate a slot index and return its base offset.

        Python's negative indexing would make set_money(-1, ...) silently write
        to the LAST slot, corrupting a save the user never asked to touch, so
        the bounds check here is a safety requirement, not a nicety.
        """
        if not isinstance(slot_index, int) or isinstance(slot_index, bool):
            raise SaveError(f"slot index must be an int, got {slot_index!r}")
        if not 0 <= slot_index < len(SLOT_OFFSETS):
            raise SaveError(
                f"slot index must be 0..{len(SLOT_OFFSETS) - 1}, got {slot_index}"
            )
        return SLOT_OFFSETS[slot_index]

    def set_money(self, slot_index: int, value: int) -> None:
        base = self._slot_base(slot_index)
        if not 0 <= value <= MONEY_MAX:
            raise SaveError(f"money must be 0..{MONEY_MAX}, got {value}")
        self._write(base + F_MONEY, struct.pack("<I", value))

    def set_party_member(
        self, slot_index: int, position: int, digimon_id: int, level: int
    ) -> None:
        """Swap a party member and set its level in BOTH stored copies.

        The party exists twice: the info-summary ``partners[3]`` (ids, what the
        load screen shows) and the data-section ``GameState.party[3]`` (partner
        INDICES, what the game actually loads -- stgmcard.c:1027). Writing only
        the summary changes the card list but leaves the running party alone, so
        this writes the summary AND the authoritative data section, converting
        the id to an index region-aware and unlocking the partner so the game
        accepts it (game3.c:830 getPartyPartner returns ``unlocked - 3``).

        The data-section write is skipped when that section is inside
        FORBIDDEN_REGIONS (in-game slots 2 and 3 today), where only the summary
        can be reached; see ``party_data_written``.
        """
        base = self._slot_base(slot_index)
        region = self.region_guess
        pmin = party_min_for_region(region)
        if not 0 <= position < PARTY_SIZE:
            raise SaveError(f"party position must be 0..2, got {position}")
        if digimon_id == 0:
            raise SaveError("party member cannot be empty; pick a base rookie")
        if not pmin <= digimon_id < pmin + 8:
            raise SaveError(
                f"party slot accepts ONLY the 8 base rookies "
                f"({region} party-space ids "
                f"{pmin}..{pmin + 7} = Kotemon..Patamon), got {digimon_id}. "
                f"Evolved forms are battle-DV mechanics and are never stored "
                f"in the party fields."
            )
        if not LEVEL_MIN <= level <= LEVEL_MAX:
            raise SaveError(f"level must be {LEVEL_MIN}..{LEVEL_MAX}, got {level}")
        # (1) info summary -- the id the load screen and card list display.
        self._write(base + F_PARTY_IDS[position], struct.pack("<I", digimon_id))
        self._write(base + F_PARTY_LEVELS[position], struct.pack("<H", level))
        # (2) data section -- the copy the game loads as the running party.
        self._write_party_data(slot_index, position, digimon_id, level)

    def _write_party_data(
        self, slot_index: int, position: int, digimon_id: int, level: int
    ) -> bool:
        """Write the data-section party copy (indices). False = section refused.

        ``GameState.party[position]`` gets the partner INDEX, the partner's
        ``unlocked`` id is set so it is accepted, and its ``STAT_LEVEL`` is set
        so the level applies in game. Returns False (no bytes written) when the
        slot's data section is inside FORBIDDEN_REGIONS.
        """
        idx = party_index_for_id(digimon_id, self.region_guess)
        ds = DATA_SECTION_OFFSETS[slot_index]
        party_off = ds + GS_PARTY + 4 * position
        partner_off = ds + DIGI_STAT_BASE_REL + idx * DIGI_STAT_STRIDE
        targets = (
            (party_off, 4),
            (partner_off + D_UNLOCK, 4),
            (partner_off + D_LEVEL, 2),
        )
        if any(self._is_forbidden(off, size) for off, size in targets):
            return False
        self._write(party_off, struct.pack("<i", idx))
        self._write(partner_off + D_UNLOCK, struct.pack("<I", digimon_id))
        self._write(partner_off + D_LEVEL, struct.pack("<H", level))
        return True

    def party_data_written(self, slot_index: int) -> bool:
        """Whether this slot's data section is outside FORBIDDEN_REGIONS."""
        self._slot_base(slot_index)                 # validate 0..2
        ds = DATA_SECTION_OFFSETS[slot_index]
        return not self._is_forbidden(ds + GS_PARTY, PARTY_SIZE * 4)

    def party_indices(self, slot_index: int) -> list[int]:
        """The AUTHORITATIVE party the game loads: partner indices, -1 = none.

        Decomp stgmcard.c:1027 (whole GameSave copied into GAME on load) and
        game_state.h:297 (``GameState.party[3]`` holds partner indices).
        """
        self._slot_base(slot_index)                 # validate 0..2
        ds = DATA_SECTION_OFFSETS[slot_index]
        return [
            struct.unpack_from("<i", self._buf, ds + GS_PARTY + 4 * i)[0]
            for i in range(PARTY_SIZE)
        ]

    def set_partner(self, slot_index: int, digimon_id: int) -> None:
        base = self._slot_base(slot_index)
        pmin = party_min_for_region(self.region_guess)
        if not pmin <= digimon_id < pmin + 8:
            raise SaveError(
                f"partner accepts ONLY the 8 base rookies "
                f"({self.region_guess} party-space ids "
                f"{pmin}..{pmin + 7} = Kotemon..Patamon), got {digimon_id}."
            )
        self._write(base + F_PARTNER, struct.pack("<I", digimon_id))

    def set_play_time(
        self, slot_index: int, hours: int, minutes: int, seconds: int
    ) -> None:
        base = self._slot_base(slot_index)
        if not 0 <= hours <= HOURS_MAX:
            raise SaveError(f"hours must be 0..{HOURS_MAX}")
        if not 0 <= minutes < 60 or not 0 <= seconds < 60:
            raise SaveError("minutes and seconds must be 0..59")
        self._write(base + F_HOURS, struct.pack("<H", hours))
        self._write(base + F_MINUTES, struct.pack("<H", minutes))
        self._write(base + F_SECONDS, struct.pack("<H", seconds))

    def set_byte(self, offset: int, value: int) -> None:
        """Raw hex-editor write, still subject to region guards."""
        if not 0 <= offset < PAYLOAD_SIZE:
            raise SaveError(f"offset out of range: {offset}")
        if not 0 <= value <= 0xFF:
            raise SaveError(f"byte value must be 0..255, got {value}")
        self._guard(offset)
        self._buf[offset] = value

    # ---- card collection (CONFIRMED 2026-09-02) -------------------------
    def card_count(self, card_index: int) -> int:
        """Copies held of one card (0..9). card_index 0..313 == card #1..#314.

        CONFIRMED: setting index 0 to 8 made the in-game collection show
        "Sacred Spear: 8" (card #1); setting index 50 to 8 showed "White
        Remove: 8" (card #51). The FAQ/research order matches the game.
        """
        if not 0 <= card_index < CARD_COUNT:
            raise SaveError(f"card index must be 0..{CARD_COUNT - 1}, got {card_index}")
        return self._buf[CARD_BASE + card_index]

    def set_card_count(self, card_index: int, count: int) -> None:
        """Set copies held of one card. count 0..9."""
        if not 0 <= card_index < CARD_COUNT:
            raise SaveError(f"card index must be 0..{CARD_COUNT - 1}, got {card_index}")
        if not 0 <= count <= CARD_MAX:
            raise SaveError(f"card count must be 0..{CARD_MAX}, got {count}")
        self._write(CARD_BASE + card_index, bytes([count]))

    def set_all_cards(self, count: int) -> None:
        """Set every card to the same count (0..9). Convenience for maxing."""
        if not 0 <= count <= CARD_MAX:
            raise SaveError(f"card count must be 0..{CARD_MAX}, got {count}")
        self._write(CARD_BASE, bytes([count]) * CARD_COUNT)

    # ---- item quantities (CONFIRMED 2026-09-02) -------------------------
    def item_qty(self, item_index: int) -> int:
        """Quantity of one item. item_index 0 == Power Charge.

        CONFIRMED 2026-09-02: slots 0-48 match the in-game item screen; the
        weapons/armor/accessories/pack subclass boundaries and name order were
        confirmed by live block + ramp tests. Layout comment above save.py's
        ITEM_BASE has the full evidence.
        """
        if not 0 <= item_index < ITEM_COUNT:
            raise SaveError(f"item index must be 0..{ITEM_COUNT - 1}, got {item_index}")
        return self._buf[ITEM_BASE + item_index]

    def set_item_qty(self, item_index: int, qty: int) -> None:
        """Set quantity of one item. qty 0..99."""
        if not 0 <= item_index < ITEM_COUNT:
            raise SaveError(f"item index must be 0..{ITEM_COUNT - 1}, got {item_index}")
        if not 0 <= qty <= ITEM_MAX:
            raise SaveError(f"item qty must be 0..{ITEM_MAX}, got {qty}")
        self._write(ITEM_BASE + item_index, bytes([qty]))

    # ---- key/important items (CONFIRMED 2026-09-02) --------------------
    def key_item_count(self, key_index: int) -> int:
        """Owned flag for one key item. key_index 0 == Tree Boots.

        Layout order = the game's Important screen (usitmnam display idx
        4..42, then Monmon DDNA, then display idx 395..402). CONFIRMED
        byte-exact against the player's 35-item read-back on 2026-09-02.
        """
        if not 0 <= key_index < KEY_ITEM_COUNT:
            raise SaveError(f"key item index must be 0..{KEY_ITEM_COUNT - 1}, got {key_index}")
        off = key_item_offset(key_index)
        return self._buf[off]

    def set_key_item(self, key_index: int, owned: int) -> None:
        """Set owned flag (0 or 1) for one key item."""
        if not 0 <= key_index < KEY_ITEM_COUNT:
            raise SaveError(f"key item index must be 0..{KEY_ITEM_COUNT - 1}, got {key_index}")
        if owned not in (0, 1):
            raise SaveError(f"key item owned must be 0 or 1, got {owned}")
        self._write(key_item_offset(key_index), bytes([owned]))

    def set_all_key_items(self, owned: int) -> None:
        """Set every key item flag to owned (1) or cleared (0)."""
        if owned not in (0, 1):
            raise SaveError(f"key item owned must be 0 or 1, got {owned}")
        for ki in range(KEY_ITEM_COUNT):
            self._write(key_item_offset(ki), bytes([owned]))

    # ---- per-Digimon current stats (CONFIRMED 2026-09-02) ---------------
    def _digi_base(self, roster_index: int) -> int:
        """Validate a roster index (0-7) and return its record offset."""
        if not isinstance(roster_index, int) or isinstance(roster_index, bool):
            raise SaveError(f"digimon index must be an int, got {roster_index!r}")
        if not 0 <= roster_index < DIGI_STAT_COUNT:
            raise SaveError(
                f"digimon index must be 0..{DIGI_STAT_COUNT - 1}, got {roster_index}"
            )
        return DIGI_STAT_BASE + roster_index * DIGI_STAT_STRIDE

    def digimon_name(self, roster_index: int) -> str:
        if not 0 <= roster_index < DIGI_STAT_COUNT:
            raise SaveError(f"digimon index must be 0..{DIGI_STAT_COUNT - 1}")
        return DIGI_ROSTER_NAMES[roster_index]

    def digimon_stats(self, roster_index: int) -> dict:
        """Read the full verified stat block for one roster digimon."""
        base = self._digi_base(roster_index)
        return {
            "exp": struct.unpack_from("<I", self._buf, base + D_EXP)[0],
            "level": struct.unpack_from("<H", self._buf, base + D_LEVEL)[0],
            "max_level": struct.unpack_from("<H", self._buf, base + D_MAX_LEVEL)[0],
            "hp": struct.unpack_from("<H", self._buf, base + D_HP)[0],
            "hp_max": struct.unpack_from("<H", self._buf, base + D_HP_MAX)[0],
            "mp": struct.unpack_from("<H", self._buf, base + D_MP)[0],
            "mp_max": struct.unpack_from("<H", self._buf, base + D_MP_MAX)[0],
            "stats": [
                struct.unpack_from("<H", self._buf, base + D_STATS + 2 * i)[0]
                for i in range(len(DIGI_STAT_NAMES))
            ],
        }

    def set_digimon_level(self, roster_index: int, level: int) -> None:
        base = self._digi_base(roster_index)
        if not 1 <= level <= DIGI_MAX_LEVEL:
            raise SaveError(f"level must be 1..{DIGI_MAX_LEVEL}, got {level}")
        self._write(base + D_LEVEL, struct.pack("<H", level))

    def set_digimon_exp(self, roster_index: int, exp: int) -> None:
        base = self._digi_base(roster_index)
        if not 0 <= exp <= DIGI_MAX_EXP:
            raise SaveError(f"exp must be 0..{DIGI_MAX_EXP}, got {exp}")
        self._write(base + D_EXP, struct.pack("<I", exp))

    def set_digimon_hp(self, roster_index: int, hp: int) -> None:
        """Set current + max HP together (they are a pair)."""
        base = self._digi_base(roster_index)
        if not 0 <= hp <= DIGI_MAX_HP:
            raise SaveError(f"HP must be 0..{DIGI_MAX_HP}, got {hp}")
        self._write(base + D_HP, struct.pack("<H", hp))
        self._write(base + D_HP_MAX, struct.pack("<H", hp))

    def set_digimon_mp(self, roster_index: int, mp: int) -> None:
        """Set current + max MP together (they are a pair)."""
        base = self._digi_base(roster_index)
        if not 0 <= mp <= DIGI_MAX_MP:
            raise SaveError(f"MP must be 0..{DIGI_MAX_MP}, got {mp}")
        self._write(base + D_MP, struct.pack("<H", mp))
        self._write(base + D_MP_MAX, struct.pack("<H", mp))

    def set_digimon_stat(self, roster_index: int, stat_index: int, value: int) -> None:
        """Set one base stat (0..12 = Strength..Dark)."""
        base = self._digi_base(roster_index)
        if not 0 <= stat_index < len(DIGI_STAT_NAMES):
            raise SaveError(f"stat index must be 0..{len(DIGI_STAT_NAMES) - 1}")
        if not 0 <= value <= DIGI_MAX_STAT:
            raise SaveError(f"stat must be 0..{DIGI_MAX_STAT}, got {value}")
        self._write(base + D_STATS + 2 * stat_index, struct.pack("<H", value))

    def set_digimon_all_stats(self, roster_index: int, value: int) -> None:
        """Set all 13 base stats to one value (0..999)."""
        if not 0 <= value <= DIGI_MAX_STAT:
            raise SaveError(f"stat must be 0..{DIGI_MAX_STAT}, got {value}")
        base = self._digi_base(roster_index)
        for i in range(len(DIGI_STAT_NAMES)):
            self._write(base + D_STATS + 2 * i, struct.pack("<H", value))

    def set_digimon_maxed(self, roster_index: int) -> None:
        """Convenience: Lv99, EXP 999999, HP/MP 9999, all stats 999."""
        base = self._digi_base(roster_index)
        data = struct.pack("<IHHHHHH", DIGI_MAX_EXP, DIGI_MAX_LEVEL, DIGI_MAX_LEVEL,
                           DIGI_MAX_HP, DIGI_MAX_HP, DIGI_MAX_MP, DIGI_MAX_MP)
        data += struct.pack("<" + "H" * len(DIGI_STAT_NAMES),
                            *([DIGI_MAX_STAT] * len(DIGI_STAT_NAMES)))
        self._write(base + D_EXP, data)

    # ---- per-digimon digivolution DV levels (CONFIRMED 2026-09-02) -------
    def _digi_dv_base(self, roster_index: int) -> int:
        """Validate a roster index (0-7) and return its record offset."""
        return self._digi_base(roster_index)

    def digimon_dv(self, roster_index: int) -> dict:
        """Read a digimon's stored digivolution DV state.

        Returns {'header': int, 'slots': [int x43], 'markers': [int x43]}.
        'header' is the primary digivolution's level (u16 @ +0x72). 'slots'
        are the 43 data-slot levels (u16 @ slot+18). 'markers' are the 43
        per-form identity u16s at slot+16..17 — each evolved form has a
        constant card-independent marker (measured 2026-09-02 from the EUR
        maxed card); marker 0 = the game has not registered that form in the
        slot, so marker is the true 'earned' signal, not the level (probe 2
        proved level alone does not make an unearned row display).
        """
        base = self._digi_dv_base(roster_index)
        header = struct.unpack_from("<H", self._buf, base + D_DV_HEADER)[0]
        slots = []
        markers = []
        for k in range(D_DV_COUNT):
            so = base + D_DV_SLOTS + k * D_DV_SLOT_STRIDE
            slots.append(struct.unpack_from("<H", self._buf, so + D_DV_SLOT_LEVEL)[0])
            markers.append(struct.unpack_from("<H", self._buf, so + 16)[0])
        return {"header": header, "slots": slots, "markers": markers}

    def dv_slot_content(self, roster_index: int, slot: int) -> bytes:
        """12-byte tech/instance record stored at slot+4..15."""
        base = self._digi_dv_base(roster_index)
        if not isinstance(slot, int) or isinstance(slot, bool) or not 0 <= slot < D_DV_COUNT:
            raise SaveError(f"DV slot must be 0..{D_DV_COUNT - 1}, got {slot!r}")
        off = base + D_DV_SLOTS + slot * D_DV_SLOT_STRIDE + D_DV_SLOT_CONTENT
        return bytes(self._buf[off:off + D_DV_SLOT_CONTENT_LEN])

    def _write_dv_display_content(self, base: int, slot: int, content: bytes) -> None:
        """Write a slot's technique record where the DV screen reads it.

        PROBE-CONFIRMED 2026-09-03: the screen shows slot k's techniques from
        the CONTENT FIELD OF SLOT k+1 (V3: zeroing slot31 removed Devimon's
        slot30 techs; V5: filling content[31..42] made rows 30..41 display all
        their techs). The header (row-1) form's techs live in slot 0's content
        field. Slot 42's display record would occupy a 43rd slot; natural cards
        (USA ri5 Sakuyamon@42) show it spilling past the record end at the
        phantom offset — 4 of the 12 bytes land in the next record's leading
        zero padding, which every record has (verified on EUR/USA).
        """
        if slot < D_DV_COUNT - 1:
            off = base + D_DV_SLOTS + (slot + 1) * D_DV_SLOT_STRIDE + D_DV_SLOT_CONTENT
        elif slot == D_DV_COUNT - 1:  # slot 42 -> phantom 43rd slot content
            off = base + D_DV_SLOT_CONTENT_PHANTOM
        else:
            raise SaveError(f"no display slot for DV slot {slot!r}")
        if not isinstance(content, (bytes, bytearray)) or len(content) != D_DV_SLOT_CONTENT_LEN:
            raise SaveError("DV display content must be exactly 12 bytes")
        self._buf[off:off + D_DV_SLOT_CONTENT_LEN] = content

    def _sync_slot_content(self, roster_index: int, slot: int) -> None:
        """Refresh one earned slot's display content from marker + DV level."""
        base = self._digi_dv_base(roster_index)
        so = base + D_DV_SLOTS + slot * D_DV_SLOT_STRIDE
        marker = struct.unpack_from("<H", self._buf, so + D_DV_SLOT_MARKER)[0]
        level = struct.unpack_from("<H", self._buf, so + D_DV_SLOT_LEVEL)[0]
        if marker and level:
            self._write_dv_display_content(base, slot, dv_tech_content(marker, level))

    def _sync_header_content(self, roster_index: int) -> None:
        """Refresh the header (row-1) form's techs in slot 0's content field."""
        base = self._digi_dv_base(roster_index)
        header = struct.unpack_from("<H", self._buf, base + D_DV_HEADER)[0]
        if not header:
            return
        name = DIGI_ROSTER_NAMES[roster_index]
        header_form = D_DV_HEADER_FORMS.get(name)
        marker = DV_FORM_MARKERS.get(header_form, 0) if header_form else 0
        if marker:
            off = base + D_DV_SLOTS + 0 * D_DV_SLOT_STRIDE + D_DV_SLOT_CONTENT
            content = dv_tech_content(marker, header)
            self._buf[off:off + D_DV_SLOT_CONTENT_LEN] = content

    def sync_digimon_dv_contents(self, roster_index: int) -> None:
        """Rewrite every earned slot's display content (natural-card behavior).

        The DV screen shows a slot's techniques from the FOLLOWING slot's
        content field. Force-earned slots previously carried zero content, so
        rows showed no moves at any DV level. Rebuilding each slot's record
        from marker + DV level (profile learn thresholds) gives every row the
        moves it would have at that level — the same bytes the game writes for
        a naturally played form.
        """
        base = self._digi_dv_base(roster_index)
        self._sync_header_content(roster_index)
        for k in range(D_DV_COUNT):
            self._sync_slot_content(roster_index, k)

    def set_digimon_dv_header(self, roster_index: int, level: int) -> None:
        """Set the primary digivolution's DV level (1..99)."""
        base = self._digi_dv_base(roster_index)
        if not 1 <= level <= D_DV_MAX:
            raise SaveError(f"DV level must be 1..{D_DV_MAX}, got {level}")
        self._write(base + D_DV_HEADER, struct.pack("<H", level))
        self._sync_header_content(roster_index)

    def set_digimon_dv_slot(self, roster_index: int, slot: int, level: int) -> None:
        """Set one data-slot DV level (slot 0..42, level 0..99).

        Level 0 marks the form as not-yet-registered; the game hides those
        rows on the DV screen regardless of the level value, because its own
        earn gate (prerequisite met -> obtain after a battle) writes the slot
        content. Use this on slots the digimon already earned.
        """
        base = self._digi_dv_base(roster_index)
        if not isinstance(slot, int) or isinstance(slot, bool) or not 0 <= slot < D_DV_COUNT:
            raise SaveError(f"DV slot must be 0..{D_DV_COUNT - 1}, got {slot!r}")
        if not isinstance(level, int) or isinstance(level, bool) or not 0 <= level <= D_DV_MAX:
            raise SaveError(f"DV level must be 0..{D_DV_MAX}, got {level!r}")
        off = base + D_DV_SLOTS + slot * D_DV_SLOT_STRIDE + D_DV_SLOT_LEVEL
        self._write(off, struct.pack("<H", level))
        if level:
            self._sync_slot_content(roster_index, slot)

    def set_digimon_dv_slot_99s(self, roster_index: int, slots: list[int]) -> None:
        """Set several data-slot DV levels to 99 in one call (earned forms)."""
        for slot in slots:
            if not isinstance(slot, int) or isinstance(slot, bool) or not 0 <= slot < D_DV_COUNT:
                raise SaveError(f"DV slot must be 0..{D_DV_COUNT - 1}, got {slot!r}")
            off = self._digi_dv_base(roster_index) + D_DV_SLOTS + slot * D_DV_SLOT_STRIDE + D_DV_SLOT_LEVEL
            self._write(off, struct.pack("<H", 99))
            self._sync_slot_content(roster_index, slot)

    def earn_digimon_dv(self, roster_index: int, form_name: str, level: int = 1,
                        slot: int | None = None) -> int:
        """Force-earn a digivolution form by writing its marker + DV level.

        CONFIRMED 2026-09-02 by the user's in-game DV screen read-back: an
        empty slot given a form's marker (u16 @ slot+16..17) and a level
        (u16 @ slot+18..19) displays that form as earned at that level.
        CONFIRMED 2026-09-03: the row's TECHNIQUES come from the *following*
        slot's content field, so earning also writes this form's canonical
        tech record there (profile learn thresholds scaled to ``level``).
        Returns the data-slot index used (auto-picks the first empty slot
        when slot is None). Header forms (row-1, e.g. Greymon for Agumon)
        live outside the slot table and cannot be earned through this path.
        """
        base = self._digi_dv_base(roster_index)
        if not isinstance(roster_index, int) or isinstance(roster_index, bool):
            raise SaveError(f"roster index must be 0..7, got {roster_index!r}")
        if not 1 <= level <= D_DV_MAX:
            raise SaveError(f"DV level must be 1..{D_DV_MAX}, got {level}")
        if form_name not in DV_FORM_MARKERS:
            raise SaveError(f"no DV form marker known for {form_name!r}")
        marker = DV_FORM_MARKERS[form_name]
        if slot is None:
            slot = self._first_empty_dv_slot(base)
            if slot is None:
                raise SaveError(
                    "all 43 DV slots already hold forms for this digimon"
                )
        if not isinstance(slot, int) or isinstance(slot, bool) or not 0 <= slot < D_DV_COUNT:
            raise SaveError(f"DV slot must be 0..{D_DV_COUNT - 1}, got {slot!r}")
        so = base + D_DV_SLOTS + slot * D_DV_SLOT_STRIDE
        self._write(so + D_DV_SLOT_MARKER, struct.pack("<H", marker))
        self._write(so + D_DV_SLOT_LEVEL, struct.pack("<H", level))
        self._write_dv_display_content(base, slot, dv_tech_content(marker, level))
        return slot

    def _first_empty_dv_slot(self, base: int) -> int | None:
        """First data slot whose marker is 0 (never earned), or None."""
        for k in range(D_DV_COUNT):
            so = base + D_DV_SLOTS + k * D_DV_SLOT_STRIDE
            marker = struct.unpack_from("<H", self._buf, so + D_DV_SLOT_MARKER)[0]
            if marker == 0:
                return k
        return None

    # ---- output ---------------------------------------------------------
    def to_bytes(self) -> bytes:
        """Serialize, recomputing every known checksum (chunks 1 and 2)."""
        out = bytearray(self._buf)
        _ck.recompute_all(out)
        return bytes(out)

    @property
    def checksum_valid(self) -> bool:
        return _ck.verify_all(self._buf)

    def summary(self) -> str:
        lines = [f"Digimon World 3 save - region {self.region_guess}"]
        for s in self.slots:
            label = f"  Slot {s.index + 1}"
            if s.is_empty:
                lines.append(f"{label}: (empty)")
                continue
            lines.append(
                f"{label}: {s.party_text} | {s.money:,} Bits | "
                f"{s.play_time_text} | area #{s.area_id} place #{s.place_id}"
            )
        return "\n".join(lines)
