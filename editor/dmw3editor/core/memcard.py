"""PS1 memory-card container I/O for the DMW3 Save Editor.

This is the foundation module.  It is stdlib + numpy only, has no GUI code,
and is fully type-hinted.

Verified against the two shipped samples (CONFIRMED by real byte inspection):

    samples/EUR_raw.mcr        raw 131072-byte card image   (region EUR)
    samples/USA_dexdrive.gme   DexDrive .gme (3904 header)  (region USA)

The two container variants that are NOT covered by a real sample
(VGS / VMP) are implemented from the documented PS1 container format and
exercised on *synthetic* containers built from the real EUR card, but no real
VGS/VMP dump was available for byte-level confirmation -> marked UNTESTED.

Endianness: PS1 is little-endian (MIPS R3000A) everywhere.
"""

from __future__ import annotations

import enum
import os
import struct
from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple

import numpy as np

# --------------------------------------------------------------------------
# Constants (CONFIRMED against samples)
# --------------------------------------------------------------------------

CARD_SIZE = 131072          # 16 blocks * 8192 bytes
BLOCK_SIZE = 8192           # one memory-card block
FRAME_SIZE = 128            # one directory frame
N_BLOCKS = 16               # blocks 0..15 (block 0 = directory)
N_DIR_FRAMES = 15           # directory frames 1..15
PAYLOAD_SIZE = 32768        # DMW3 save = exactly 4 blocks (CONFIRMED)

# Container headers
GME_HEADER = 3904           # DexDrive .gme header length (CONFIRMED: 134976-131072)
VGS_HEADER = 128           # Connectix/VGS -mem header length (UNTESTED - from spec)
VMP_HEADER = 128           # PSP/PS3 .vmp exported header length (UNTESTED - from spec)

# Directory frame `state` field values (CONFIRMED)
STATE_FIRST = 0x51          # first block of a save
STATE_MIDDLE = 0x52         # middle block
STATE_LAST = 0x53           # last block
STATE_FREE = 0xA0           # free / unused

# DMW3 save name prefixes -> region.
# USA/EUR CONFIRMED from real sample cards. All THREE ids were additionally
# confirmed from the game's own executable SLES_039.36 (extracted from the EUR
# disc), where they sit adjacent as ASCII at file offsets 0x9FC / 0xA14 / 0xA2C:
#   BISLPS-03446DMW3-JPN / BASLUS-01436DMW3-USA / BESLES-03936DMW3-EUR
# JPN is implemented from that authoritative binary evidence but UNTESTED
# against a real JPN card (we have no JPN sample).
DMW3_PREFIXES: Dict[str, str] = {
    "BASLUS-01436": "USA",
    "BESLES-03936": "EUR",
    "BISLPS-03446": "JPN",
}

# PS1 save ("SC") title-frame layout, offsets within the payload.
SC_MAGIC = b"SC"
SC_TITLE_OFF = 0x04         # 48-byte Shift-JIS title (CONFIRMED)
SC_TITLE_LEN = 48
SC_CLUT_OFF = 0x60          # 16 * u16 RGB555 CLUT (CONFIRMED)
SC_ICON_OFF = 0x80          # 16x16 4bpp icon frames (CONFIRMED)
SC_CLUT_N = 16
ICON_W = 16
ICON_H = 16
ICON_FRAME_BYTES = ICON_W * ICON_H // 2  # 128 bytes per 16x16 4bpp frame


class ContainerFormat(str, enum.Enum):
    RAW = "raw"     # .mcr/.mcd/.mc/.bin/.srm  -> 131072 bytes, no header
    GME = "gme"     # DexDrive .gme            -> 3904 header + body
    VGS = "vgs"     # Connectix/VGS .mem       -> 128 header + body (UNTESTED)
    VMP = "vmp"     # PSP/PS3 .vmp export      -> 128 header + body (UNTESTED)


# --------------------------------------------------------------------------
# Data structures
# --------------------------------------------------------------------------

@dataclass
class DirEntry:
    """One 128-byte directory frame (block 0, frames 1..15)."""
    block: int            # frame index (1..15) this entry lives at
    state: int            # 0x51/0x52/0x53/0xA0
    size: int             # size in bytes field (0 for linked middle/last)
    next_block: int       # next-block pointer field
    name: str             # raw 20-byte name, nul-stripped


@dataclass
class SaveLocation:
    """Where the DMW3 save lives on the card."""
    name: str             # full save name, e.g. BASLUS-01436DMW3-USA
    region: str           # "USA" / "EUR"
    prefix: str           # matched prefix
    first_block: int      # block index of the first block (1 for DMW3)
    chain: List[int]      # ordered block indices (e.g. [1,2,3,4])
    size: int             # declared save size in bytes (32768)


@dataclass
class TitleFrame:
    """Parsed PS1 'SC' title frame (block 1, payload offset 0)."""
    magic: str                          # "SC"
    icon_flags: int                     # raw icon-flag byte (0x13 observed)
    icon_count: int                     # number of animated icon frames (4 observed)
    title: str                          # decoded Shift-JIS title
    clut: np.ndarray = field(repr=False)        # (16, 3) uint8 RGB
    icon_frames: np.ndarray = field(repr=False) # (icon_count, 16, 16, 3) uint8 RGB


# --------------------------------------------------------------------------
# Container detection
# --------------------------------------------------------------------------

def detect_format(raw: bytes) -> Tuple[ContainerFormat, int]:
    """Return (format, header_len) for a raw on-disk container.

    Detection is by total size, with a VMP magic check to disambiguate the
    128-byte-header family (VGS vs VMP).  The .gme header is all-zero, so it
    is identified by size only (134976 = 131072 + 3904, CONFIRMED).
    """
    total = len(raw)
    if total == CARD_SIZE:
        return ContainerFormat.RAW, 0
    if total == CARD_SIZE + GME_HEADER:
        return ContainerFormat.GME, GME_HEADER
    if total == CARD_SIZE + VGS_HEADER:
        # 128-byte-header family.  VMP starts with the "VMP" magic.
        if raw[:3] == b"VMP":
            return ContainerFormat.VMP, VMP_HEADER
        return ContainerFormat.VGS, VGS_HEADER
    raise ValueError(
        f"Unrecognised container size {total} bytes "
        f"(expected {CARD_SIZE}, {CARD_SIZE+GME_HEADER}, or {CARD_SIZE+VGS_HEADER})"
    )


# --------------------------------------------------------------------------
# MemoryCard
# --------------------------------------------------------------------------

class MemoryCard:
    """Loads, inspects, and rewrites a PS1 memory-card image in any container.

    The in-memory model keeps the original container *header* verbatim and the
    full 131072-byte card *body* as a mutable bytearray.  Reinserting an
    unchanged payload therefore reproduces the original file byte-for-byte.
    """

    def __init__(self, raw: bytes):
        self.format, self.header_len = detect_format(raw)
        self.header = bytes(raw[:self.header_len])      # verbatim, preserved
        body = raw[self.header_len:]
        if len(body) != CARD_SIZE:
            # Defensive: shouldn't happen after detect_format, but be explicit.
            raise ValueError(f"Card body is {len(body)} bytes, expected {CARD_SIZE}")
        self.card: bytearray = bytearray(body)

    # -- constructors ----------------------------------------------------
    @classmethod
    def load(cls, path: str) -> "MemoryCard":
        with open(path, "rb") as f:
            return cls(f.read())

    # -- directory -------------------------------------------------------
    def _frame(self, block: int) -> bytes:
        """Return the 128-byte directory frame for `block` (1..15)."""
        return bytes(self.card[block * FRAME_SIZE:(block + 1) * FRAME_SIZE])

    def directory(self) -> List[DirEntry]:
        """Parse all 15 directory frames (block 0, frames 1..15)."""
        entries: List[DirEntry] = []
        for b in range(1, N_BLOCKS):
            fr = self._frame(b)
            state, size, nxt = struct.unpack_from("<IIH", fr, 0)
            name_raw = fr[10:30]
            name = name_raw.split(b"\x00", 1)[0].decode("ascii", "replace")
            entries.append(DirEntry(b, state, size, nxt, name))
        return entries

    def _follow_chain(self, first: int) -> List[int]:
        """Follow the block chain from `first`.

        Uses the directory `next_block` pointer.  The shipped DMW3 saves use a
        degenerate directory (next_block == own index for 0x51/0x52 frames), so
        on a self-pointer/cycle we fall back to contiguous blocks -- which is
        what the PS1 allocator always produces anyway (CONFIRMED: size 32768
        == 4 * 8192 starting at block 1).
        """
        chain = [first]
        visited = {first}
        cur = first
        while True:
            frame = self._frame(cur)
            if len(frame) < 10:
                # Corrupt/truncated directory frame: stop rather than crash.
                break
            nxt = struct.unpack("<H", frame[8:10])[0]
            if nxt in (0, 0xFFFF) or nxt > N_BLOCKS - 1:
                break
            if nxt in visited:
                # Degenerate self/cycle -> contiguous fallback.
                cand = cur + 1
                if cand > N_BLOCKS - 1 or cand in visited:
                    break
                if self._frame(cand)[0] in (STATE_MIDDLE, STATE_LAST):
                    nxt = cand
                else:
                    break
            chain.append(nxt)
            visited.add(nxt)
            cur = nxt
            if len(chain) >= N_DIR_FRAMES:
                break
        return chain

    # -- DMW3 save -------------------------------------------------------
    def find_dmw3_save(self) -> Optional[SaveLocation]:
        """Locate the DMW3 save by name prefix; return region + block chain."""
        for e in self.directory():
            for prefix, region in DMW3_PREFIXES.items():
                if e.name.startswith(prefix):
                    chain = self._follow_chain(e.block)
                    return SaveLocation(
                        name=e.name,
                        region=region,
                        prefix=prefix,
                        first_block=e.block,
                        chain=chain,
                        size=e.size,
                    )
        return None

    def extract_payload(self, loc: Optional[SaveLocation] = None) -> bytes:
        """Extract the DMW3 32768-byte payload by following the block chain."""
        if loc is None:
            loc = self.find_dmw3_save()
            if loc is None:
                raise ValueError("No DMW3 save found on this card")
        out = bytearray()
        for blk in loc.chain:
            out += self.card[blk * BLOCK_SIZE:(blk + 1) * BLOCK_SIZE]
        return bytes(out)

    def reinsert_payload(self, payload: bytes,
                         loc: Optional[SaveLocation] = None) -> None:
        """Write `payload` back into the original block chain, in place.

        Length must equal len(chain) * BLOCK_SIZE (32768 for DMW3).  Only the
        save's data blocks are touched; the directory and all other blocks are
        left byte-identical, so re-saving reproduces the original file.
        """
        if loc is None:
            loc = self.find_dmw3_save()
            if loc is None:
                raise ValueError("No DMW3 save found on this card")
        expected = len(loc.chain) * BLOCK_SIZE
        if len(payload) != expected:
            raise ValueError(
                f"payload is {len(payload)} bytes, expected {expected} "
                f"({len(loc.chain)} blocks * {BLOCK_SIZE})"
            )
        for i, blk in enumerate(loc.chain):
            self.card[blk * BLOCK_SIZE:(blk + 1) * BLOCK_SIZE] = \
                payload[i * BLOCK_SIZE:(i + 1) * BLOCK_SIZE]

    # -- title frame -----------------------------------------------------
    def parse_title(self, payload: Optional[bytes] = None) -> TitleFrame:
        """Parse the PS1 'SC' title frame at the start of the payload."""
        if payload is None:
            payload = self.extract_payload()
        if payload[:2] != SC_MAGIC:
            raise ValueError(
                f"Payload does not start with 'SC' (got {payload[:2]!r})"
            )
        icon_flags = payload[2]
        icon_count = payload[3]
        title_raw = payload[SC_TITLE_OFF:SC_TITLE_OFF + SC_TITLE_LEN]
        nul = title_raw.find(b"\x00")
        if nul != -1:
            title_raw = title_raw[:nul]
        title = title_raw.decode("cp932", errors="replace")

        # CLUT: 16 * u16 RGB555 little-endian at 0x60.
        clut_raw = payload[SC_CLUT_OFF:SC_CLUT_OFF + SC_CLUT_N * 2]
        clut_u16 = np.frombuffer(clut_raw, dtype="<u2").astype(np.uint32)
        r = ((clut_u16 & 0x1F) * 255 // 31).astype(np.uint8)
        g = (((clut_u16 >> 5) & 0x1F) * 255 // 31).astype(np.uint8)
        b = (((clut_u16 >> 10) & 0x1F) * 255 // 31).astype(np.uint8)
        clut = np.stack([r, g, b], axis=1)  # (16, 3)

        # Icon frames: icon_count frames of 128 bytes (16x16 4bpp).
        icon_frames = self._decode_icons(payload, icon_count, clut)
        return TitleFrame(
            magic="SC",
            icon_flags=icon_flags,
            icon_count=icon_count,
            title=title,
            clut=clut,
            icon_frames=icon_frames,
        )

    @staticmethod
    def _decode_icons(payload: bytes, icon_count: int,
                      clut: np.ndarray) -> np.ndarray:
        """Decode 4bpp icon frames into (icon_count, 16, 16, 3) uint8 RGB."""
        frames = []
        for f in range(icon_count):
            off = SC_ICON_OFF + f * ICON_FRAME_BYTES
            blob = payload[off:off + ICON_FRAME_BYTES]
            if len(blob) < ICON_FRAME_BYTES:
                # Truncated frame -> zero-filled placeholder (defensive).
                blob = blob + b"\x00" * (ICON_FRAME_BYTES - len(blob))
            idx = np.frombuffer(blob, dtype=np.uint8)
            # Each byte holds two pixels: high nibble = left, low nibble = right.
            pix = np.empty(ICON_W * ICON_H, dtype=np.uint8)
            pix[0::2] = (idx >> 4) & 0xF
            pix[1::2] = idx & 0xF
            pix = pix.reshape(ICON_H, ICON_W)
            frames.append(clut[pix])
        if frames:
            return np.stack(frames, axis=0)
        return np.zeros((0, ICON_H, ICON_W, 3), dtype=np.uint8)

    # -- serialise -------------------------------------------------------
    def save(self, path: str) -> None:
        """Write the container back out in the ORIGINAL format (header+body)."""
        with open(path, "wb") as f:
            f.write(self.header)
            f.write(self.card)


# --------------------------------------------------------------------------
# Convenience helpers (used by tests / other agents)
# --------------------------------------------------------------------------

def load_card(path: str) -> MemoryCard:
    return MemoryCard.load(path)


def extract_dmw3_payload(path: str) -> Tuple[bytes, SaveLocation]:
    """One-shot: load a container, return (payload, save_location)."""
    mc = MemoryCard.load(path)
    loc = mc.find_dmw3_save()
    if loc is None:
        raise ValueError("No DMW3 save found")
    return mc.extract_payload(loc), loc
