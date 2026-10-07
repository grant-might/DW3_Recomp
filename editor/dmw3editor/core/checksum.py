"""DMW3 memory-card save integrity — the two checksummed sections.

Authority: the byte-matching decompilation
(``include/stgmcard.h``, ``src/main/memcard.c``, ``src/stgmcard/stgmcard.c``),
cross-checked byte-for-byte against real USA and European cards.

The DMW3 card file is 4 blocks (32,768 bytes) laid out as::

    0x0000..0x0200   PS1 title frame ('SC' header + 3 icon frames, 128 B each)
    0x0200..0x0300   the info section: MemCardFile (0xD4 B) + 0x2C zero padding
    0x0300..0x2A00   data section 0 (one GameSave; section stride 0x2700)
    0x2A00..0x5100   data section 1
    0x5100..0x7800   data section 2
    0x7800..0x8000   trailing 0x800 bytes (not written by the game; unidentified)

The info section (``MemCardFile``) header, payload-absolute::

    0x0200  u8   checksum  = XOR8 over [0x0204, 0x02D4)  ("of the rest, from magic")
    0x0201  u8   last      the slot last saved to (0..2)
    0x0202  u8   version   MEMCARD_SAVE_VERSION (3 USA, 4 EUR)
    0x0203  u8   unk3      (0 on both regions)
    0x0204  s32  magic     "DMW3"
    0x0208  MemCardSave saves[3]  (0x44 bytes each)

Every data section (``GameSave``) header, relative to its base (0x0300 / 0x2A00 /
0x5100)::

    +0x00  u8   checksum  = XOR8 over [+0x04, +0x04 + GAME_SAVE_SIZE - 4)
    +0x01  u8   (unused; zero)
    +0x02  u8   version   MEMCARD_SAVE_VERSION
    +0x03  u8   (unused; zero)
    +0x04  ...  the game state (GameState's first GAME_SAVE_SIZE bytes)

GAME_SAVE_SIZE is REGION-SPECIFIC (``stgmcard.h``): 0x26BC on USA, 0x26C4 on EUR.
The game writes/reads ``sizeof(GameSave)`` bytes and checksums all but its first
four, so the covered end is::

    USA :  0x0304 + 0x26BC - 4 = 0x29BC
    EUR :  0x0304 + 0x26C4 - 4 = 0x29C4

That difference is REAL and observable: the live USA card stores data-section
checksum 0xBA, which equals XOR8 over [0x0304, 0x29BC) but NOT over
[0x0304, 0x29C4) (0xBD), because that card has non-zero bytes in
[0x29BC, 0x29C4). The editor previously used the single EUR extent 0x29C4 for
every card, so it mis-verified — and would have REWRITTEN — a live USA save's
data-section checksum. The extent is now selected from the version byte actually
present in the payload.

Both stored checksums are single BYTES (the game's ``computeChecksum`` returns
``u8``); 0x0201/0x0203 and the data section's +0x01/+0x03 are separate fields.
The editor previously read/wrote the checksums as u16 and so silently zeroed the
``last`` byte at 0x0201 on every save; it now writes exactly the checksum byte.
"""
from __future__ import annotations

from functools import reduce
from operator import xor

# ---- Section 1: the info section (MemCardFile) at payload 0x0200 ----------
HEADER_OFFSET = 0x0200
CHECKSUM_OFFSET = HEADER_OFFSET + 0x00   # 0x0200  u8
LAST_OFFSET = HEADER_OFFSET + 0x01       # 0x0201  u8  slot last saved to
VERSION_OFFSET = HEADER_OFFSET + 0x02    # 0x0202  u8
UNK3_OFFSET = HEADER_OFFSET + 0x03       # 0x0203  u8
MAGIC_OFFSET = HEADER_OFFSET + 0x04      # 0x0204  s32 "DMW3"
COVERED_START = MAGIC_OFFSET             # 0x0204

MEMCARD_FILE_SIZE = 0x00D4               # sizeof(MemCardFile)
COVERED_END = HEADER_OFFSET + MEMCARD_FILE_SIZE  # 0x02D4 (exclusive)

# ---- Sections 2..4: data sections (GameSave) ------------------------------
# The first data section begins right after the 0x100-byte info section; the
# three sections are MEMCARD.dataSize (0x2700) apart (system.c initMemCard:
# MEMCARD.infoSize = 0x100, MEMCARD.dataSize = 0x2700).
CHUNK2_OFFSET = 0x0300                   # data section 0 base
CHUNK2_CHECKSUM_OFFSET = CHUNK2_OFFSET + 0x00   # u8
CHUNK2_VERSION_OFFSET = CHUNK2_OFFSET + 0x02    # u8
CHUNK2_COVERED_START = CHUNK2_OFFSET + 0x04     # 0x0304
DATA_SECTION_OFFSETS = (0x0300, 0x2A00, 0x5100)
DATA_SECTION_SIZE = 0x2700

# GAME_SAVE_SIZE by MEMCARD_SAVE_VERSION (stgmcard.h).
GAME_SAVE_SIZE_BY_REGION = {3: 0x26BC, 4: 0x26C4}
# Covered end = CHUNK2_COVERED_START + GAME_SAVE_SIZE - 4.
CHUNK2_COVERED_END_USA = 0x29BC          # 0x0304 + 0x26BC - 4
CHUNK2_COVERED_END = 0x29C4              # EUR (0x0304 + 0x26C4 - 4), the max

KNOWN_VERSIONS = {3: "USA-style (BASLUS-01436)", 4: "EUR-style (BESLES-03936)"}

# Kept for the region-agnostic "past the covered data" warning; see
# unverified_regions(). A third header shape does follow each data section.
UNSOLVED_CHUNK_HEADER = 0x0300


def xor8(data: bytes) -> int:
    """XOR-fold every byte. Returns 0 for empty input."""
    return reduce(xor, data, 0)


def region_version(payload: bytes) -> int:
    """The save's format version byte at 0x0202 (same value at 0x0302)."""
    return payload[VERSION_OFFSET]


def compute_header_checksum(payload: bytes) -> int:
    """The checksum the header SHOULD hold, given current payload bytes."""
    if len(payload) < COVERED_END:
        raise ValueError(
            f"payload too short: need >= {COVERED_END}, got {len(payload)}"
        )
    return xor8(payload[COVERED_START:COVERED_END])


def stored_header_checksum(payload: bytes) -> int:
    """The stored checksum byte at 0x0200 (u8, not u16)."""
    return payload[CHECKSUM_OFFSET]


def stored_header_last(payload: bytes) -> int:
    """The 'slot last saved to' byte at 0x0201 (MemCardFile.last)."""
    return payload[LAST_OFFSET]


def stored_unk3(payload: bytes) -> int:
    """The unidentified byte at 0x0203 (MemCardFile.unk3; 0 on both regions)."""
    return payload[UNK3_OFFSET]


def format_version(payload: bytes) -> int:
    """The format version byte: 3 = USA, 4 = EUR (MEMCARD_SAVE_VERSION)."""
    return payload[VERSION_OFFSET]


def verify_header(payload: bytes) -> bool:
    """True when the stored header checksum matches the data."""
    return stored_header_checksum(payload) == compute_header_checksum(payload)


def recompute_header(payload: bytearray) -> int:
    """Write the correct header checksum in place. Returns the new value.

    Call this after ANY edit to bytes in [0x0204, 0x02D4). Only 0x0200 is
    written, so the ``last`` byte at 0x0201 is preserved.
    """
    value = compute_header_checksum(payload)
    payload[CHECKSUM_OFFSET] = value
    return value


# ---- chunk 2: a data section (GameSave) ----------------------------------
def chunk2_covered_end(payload: bytes) -> int:
    """Exclusive end of the region's data-section checksum range.

    USA = 0x29BC, EUR = 0x29C4. Falls back to the EUR (maximum) extent for an
    unknown version byte, which only ever over-covers — it can neither hide an
    edit nor mis-verify a card the game would accept.
    """
    size = GAME_SAVE_SIZE_BY_REGION.get(region_version(payload), 0x26C4)
    return CHUNK2_COVERED_START + size - 4


def compute_chunk2_checksum(payload: bytes) -> int:
    """The data-section checksum its header SHOULD hold for this region."""
    end = chunk2_covered_end(payload)
    if len(payload) < end:
        raise ValueError(
            f"payload too short: need >= {end}, got {len(payload)}"
        )
    return xor8(payload[CHUNK2_COVERED_START:end])


def stored_chunk2_checksum(payload: bytes) -> int:
    """The stored data-section checksum byte at 0x0300 (u8, not u16)."""
    return payload[CHUNK2_CHECKSUM_OFFSET]


def verify_chunk2(payload: bytes) -> bool:
    """True when the stored data-section checksum matches the data."""
    return stored_chunk2_checksum(payload) == compute_chunk2_checksum(payload)


def recompute_chunk2(payload: bytearray) -> int:
    """Write the correct data-section checksum byte in place. Returns it.

    Call this after ANY edit to bytes in the region's covered range
    ([0x0304, 0x29BC) on USA, [0x0304, 0x29C4) on EUR) — this includes the
    item quantity array at 0x03A7 and the card collection at 0x06A3. Only
    0x0300 is written.
    """
    value = compute_chunk2_checksum(payload)
    payload[CHUNK2_CHECKSUM_OFFSET] = value
    return value


def recompute_all(payload: bytearray) -> None:
    """Recompute every known checksum in place (chunks 1 and 2)."""
    recompute_header(payload)
    recompute_chunk2(payload)


def verify_all(payload: bytes) -> bool:
    """True when every known checksum validates."""
    return verify_header(payload) and verify_chunk2(payload)


def unverified_regions(payload: bytes) -> list[tuple[int, int, str]]:
    """Regions an editor can change but whose integrity we cannot guarantee.

    Returned so the GUI can warn the user instead of silently risking a save.
    The start is the module-level :data:`CHUNK2_COVERED_END` (0x29C4), the
    maximum covered end across regions. On a USA card the exact covered end is
    0x29BC, so [0x29BC, 0x29C4) is also uncovered there; it lies inside the
    editor's FORBIDDEN_REGIONS and is never written, so the conservative start
    is kept.
    """
    return [
        (
            CHUNK2_COVERED_END,
            len(payload),
            "No checksum is known to cover this region. A third chunk header "
            "shape appears right after 0x29C4 but its covered extent is not "
            "yet determined. Back up your card before editing here.",
        )
    ]
