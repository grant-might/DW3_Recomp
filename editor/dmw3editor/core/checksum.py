"""DMW3 save integrity, the CHUNKED checksum layout at payload 0x0200.

CONFIRMED, independently validated on three cards (USA, EUR-before,
EUR-after-item-buy):

    u16 @ 0x0200 (little-endian) == XOR of every byte in [0x0204, 0x0300)
    u16 @ 0x0300 (little-endian) == XOR of every byte in [0x0304, 0x29C4)

    Chunk 1: USA stored 232 computed 232; EUR before 90/90; EUR after 255/255
    Chunk 2: USA stored 140 computed 140; EUR before 73/73; EUR after 189/189

Chunk headers share the same shape:
    +0x00  u16  xor8 checksum over [+0x04, chunk_end)
    +0x02  u16  format/version (3 on the USA card, 4 on the EUR card)
    +0x04  ...  data ('DMW3' tag opens chunk 1's data)

Why chunk 2 is real (2026-09-02 discovery via a controlled in-game diff):
  * Chunk 2's checksum CHANGED when the player bought one item (0x49 -> 0xBD
    on the EUR card) because the item quantity array lives at 0x03A7, inside
    chunk 2. Any edit to the item inventory MUST recompute this.
  * It validates on THREE independent card states with different stored values.

The earlier claim in this file that the format is NOT chunked is WRONG -
it was based on testing extents only from 0x0204 and fixed 0x100 pages. The
chunk-2 extent [0x0304, 0x29C4) was found by solving the common end across
three cards and is far too specific (three exact matches) to be coincidence.

CONSEQUENCE FOR SAFETY: two checksums to keep in sync.
  * Edits to [0x0204, 0x0300)  -> recompute u16@0x0200.
  * Edits to [0x0304, 0x29C4) -> recompute u16@0x0300.  (includes items!)
  * A further chunk header shape appears after 0x29C4 (0x29C0 region shows a
    fresh checksum+version on the EUR-after card); its extent is not yet
    determined.
"""
from __future__ import annotations

import struct
from functools import reduce
from operator import xor

HEADER_OFFSET = 0x0200
CHECKSUM_OFFSET = HEADER_OFFSET + 0x00
VERSION_OFFSET = HEADER_OFFSET + 0x02
COVERED_START = HEADER_OFFSET + 0x04      # 0x0204
COVERED_END = HEADER_OFFSET + 0x100       # 0x0300 (exclusive)

# Chunk 2 (discovered 2026-09-02): header at 0x0300, covers [0x0304, 0x29C4).
CHUNK2_OFFSET = 0x0300
CHUNK2_COVERED_START = CHUNK2_OFFSET + 0x04
CHUNK2_COVERED_END = 0x29C4  # exclusive

KNOWN_VERSIONS = {3: "USA-style (BASLUS-01436)", 4: "EUR-style (BESLES-03936)"}

# Later headers matching the (u16 checksum, u16 version) shape whose covered
# extent we have NOT determined. Editing inside these is not checksum-safe.
UNSOLVED_CHUNK_HEADER = 0x0300


def xor8(data: bytes) -> int:
    """XOR-fold every byte. Returns 0 for empty input."""
    return reduce(xor, data, 0)


def compute_header_checksum(payload: bytes) -> int:
    """The checksum the header SHOULD hold, given current payload bytes."""
    if len(payload) < COVERED_END:
        raise ValueError(
            f"payload too short: need >= {COVERED_END}, got {len(payload)}"
        )
    return xor8(payload[COVERED_START:COVERED_END])


def stored_header_checksum(payload: bytes) -> int:
    return struct.unpack_from("<H", payload, CHECKSUM_OFFSET)[0]


def format_version(payload: bytes) -> int:
    return struct.unpack_from("<H", payload, VERSION_OFFSET)[0]


def verify_header(payload: bytes) -> bool:
    """True when the stored header checksum matches the data."""
    return stored_header_checksum(payload) == compute_header_checksum(payload)


def recompute_header(payload: bytearray) -> int:
    """Write the correct header checksum in place. Returns the new value.

    Call this after ANY edit to bytes in [0x0204, 0x0300).
    """
    value = compute_header_checksum(payload)
    struct.pack_into("<H", payload, CHECKSUM_OFFSET, value)
    return value


def compute_chunk2_checksum(payload: bytes) -> int:
    """The chunk-2 checksum the header at 0x0300 SHOULD hold."""
    if len(payload) < CHUNK2_COVERED_END:
        raise ValueError(
            f"payload too short: need >= {CHUNK2_COVERED_END}, got {len(payload)}"
        )
    return xor8(payload[CHUNK2_COVERED_START:CHUNK2_COVERED_END])


def stored_chunk2_checksum(payload: bytes) -> int:
    return struct.unpack_from("<H", payload, CHUNK2_OFFSET)[0]


def verify_chunk2(payload: bytes) -> bool:
    """True when the stored chunk-2 checksum matches the data."""
    return stored_chunk2_checksum(payload) == compute_chunk2_checksum(payload)


def recompute_chunk2(payload: bytearray) -> int:
    """Write the correct chunk-2 checksum in place. Returns the new value.

    Call this after ANY edit to bytes in [0x0304, 0x29C4), this includes the
    item quantity array at 0x03A7.
    """
    value = compute_chunk2_checksum(payload)
    struct.pack_into("<H", payload, CHUNK2_OFFSET, value)
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
