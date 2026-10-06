"""DMW3 save-file domain model.

Everything here is derived from CONFIRMED byte evidence (see docs/ADDENDUM_V2.md).
Speculative field offsets live in FIELDS with a confidence flag and are surfaced
in the GUI as 'unverified' until research promotes them.
"""
from __future__ import annotations

import struct
from dataclasses import dataclass, field
from typing import Iterator

# ---- CONFIRMED layout constants -------------------------------------------

PAYLOAD_SIZE = 32768

# CONFIRMED (docs/CORRECTION_V3.md): the payload holds ONE logical save record
# at 0x0200, plus up to two REDUNDANT COPIES / earlier revisions at 0x2900 and
# 0x5000 that exist only on some cards (present on USA, absent on EUR).
# The ASCII "DMW3" tag occurs exactly ONCE, at 0x0204, in the primary record.
RECORD_BASE = 0x0200
RECORD_SIZE = 0x2700
BACKUP_BASES = (0x2900, 0x5000)
ALL_BASES = (RECORD_BASE,) + BACKUP_BASES

RECORD_TAG_OFFSET = 0x04
RECORD_TAG = b"DMW3"

# Back-compat aliases (deprecated names from the withdrawn three-slot model).
SLOT_SIZE = RECORD_SIZE
SLOT_BASES = ALL_BASES
SLOT_TAG_OFFSET = RECORD_TAG_OFFSET
SLOT_TAG = RECORD_TAG

PARTY_OFFSET = 0x08          # relative to slot base
PARTY_STRIDE = 0x44          # 68 bytes
PARTY_COUNT = 3

REGION_USA = "USA"
REGION_EUR = "EUR"
SAVE_NAMES = {
    "BASLUS-01436": REGION_USA,
    "BESLES-03936": REGION_EUR,
}


class SaveError(Exception):
    """Raised when a payload is not a recognisable DMW3 save."""


@dataclass
class PartyMember:
    """One 68-byte active-party record.

    Field semantics are still being confirmed; raw access is always available so
    the editor is never blocked on incomplete research.
    """

    index: int
    raw: bytearray

    def u8(self, off: int) -> int:
        return self.raw[off]

    def u16(self, off: int) -> int:
        return struct.unpack_from("<H", self.raw, off)[0]

    def u32(self, off: int) -> int:
        return struct.unpack_from("<I", self.raw, off)[0]

    def set_u8(self, off: int, val: int) -> None:
        self.raw[off] = val & 0xFF

    def set_u16(self, off: int, val: int) -> None:
        struct.pack_into("<H", self.raw, off, val & 0xFFFF)

    def set_u32(self, off: int, val: int) -> None:
        struct.pack_into("<I", self.raw, off, val & 0xFFFFFFFF)


@dataclass
class SaveRecord:
    """One save record: the primary at 0x0200, or a redundant backup copy.

    Only the PRIMARY carries the `DMW3` tag and the `E8 00 03 00` header; backup
    copies begin differently and are NOT structured identically (see
    docs/CORRECTION_V3.md). Field offsets are only known-valid for the primary.
    """

    index: int
    base: int
    raw: bytearray

    @property
    def is_primary(self) -> bool:
        return self.base == RECORD_BASE

    @property
    def occupied(self) -> bool:
        """True when this record carries the primary DMW3 tag."""
        return bytes(self.raw[RECORD_TAG_OFFSET:RECORD_TAG_OFFSET + 4]) == RECORD_TAG

    @property
    def party(self) -> list[PartyMember]:
        out = []
        for i in range(PARTY_COUNT):
            start = PARTY_OFFSET + i * PARTY_STRIDE
            out.append(PartyMember(i, self.raw[start:start + PARTY_STRIDE]))
        return out

    def write_party_member(self, member: PartyMember) -> None:
        start = PARTY_OFFSET + member.index * PARTY_STRIDE
        if len(member.raw) != PARTY_STRIDE:
            raise SaveError(f"party record must be {PARTY_STRIDE} bytes")
        self.raw[start:start + PARTY_STRIDE] = member.raw


@dataclass
class DMW3Save:
    """The 32,768-byte DMW3 payload: a title frame plus three save slots."""

    payload: bytearray
    region: str = REGION_USA
    title_frame: bytes = field(default=b"", repr=False)

    def __post_init__(self) -> None:
        if len(self.payload) != PAYLOAD_SIZE:
            raise SaveError(
                f"payload must be {PAYLOAD_SIZE} bytes, got {len(self.payload)}"
            )
        self.title_frame = bytes(self.payload[:SLOT_BASES[0]])

    @classmethod
    def from_bytes(cls, data: bytes, region: str = REGION_USA) -> "DMW3Save":
        return cls(bytearray(data), region)

    @property
    def primary(self) -> SaveRecord:
        """The live save record, this is what the editor edits."""
        return SaveRecord(
            0, RECORD_BASE,
            self.payload[RECORD_BASE:RECORD_BASE + RECORD_SIZE],
        )

    @property
    def records(self) -> list[SaveRecord]:
        """Primary plus any redundant backup copies present in this payload."""
        out = []
        for i, base in enumerate(ALL_BASES):
            if base + RECORD_SIZE > PAYLOAD_SIZE:
                continue
            out.append(
                SaveRecord(i, base, self.payload[base:base + RECORD_SIZE])
            )
        return out

    def backup_copies_present(self, min_fill: float = 0.10) -> bool:
        """True when a real redundant copy exists, not just stray bytes.

        EUR cards carry ~99 non-zero bytes past 0x2900 (stray tail data, well
        under 1% fill) while USA cards carry genuine full copies, so a bare
        "any non-zero" test gives a false positive on EUR. Require the region to
        be at least `min_fill` populated.
        """
        for base in BACKUP_BASES:
            region = self.payload[base:base + RECORD_SIZE]
            if not region:
                continue
            if sum(1 for b in region if b) / len(region) >= min_fill:
                return True
        return False

    def write_record(self, record: SaveRecord) -> None:
        if len(record.raw) != RECORD_SIZE:
            raise SaveError(f"record must be {RECORD_SIZE} bytes")
        self.payload[record.base:record.base + RECORD_SIZE] = record.raw

    def mirror_primary_to_backups(self) -> int:
        """Copy the primary over both backup regions.

        UNRESOLVED RISK: it is not yet confirmed whether the game requires the
        backup copies to match, nor whether they share the primary's layout.
        The GUI must expose this as an explicit opt-in, never do it silently.
        """
        src = self.payload[RECORD_BASE:RECORD_BASE + RECORD_SIZE]
        n = 0
        for base in BACKUP_BASES:
            if base + RECORD_SIZE <= PAYLOAD_SIZE:
                self.payload[base:base + RECORD_SIZE] = src
                n += 1
        return n

    def to_bytes(self) -> bytes:
        return bytes(self.payload)
