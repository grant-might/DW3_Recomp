"""Controlled-diff helper for locating UNVERIFIED save fields.

WHY THIS EXISTS
---------------
The item inventory, card collection, and per-Digimon stat/XP blocks were not
found by exhaustive static search (see docs/OPEN_LEADS.md): two unrelated
player saves produce thousands of statistically indistinguishable candidates.
The only reliable way to locate them is a CONTROLLED DIFF, two saves of the
SAME playthrough that differ by exactly ONE in-game action (e.g. "use one
Potion", "give 1 item to a Digimon"). The bytes that change then are the field.

This module is READ-ONLY analysis. It never writes a save and never feeds a
guess into the editable model. It classifies changed bytes against the KNOWN
verified fields and reports everything else as an "unknown region" a human can
inspect in-game.

A test changes exactly the money field and asserts the diff reports ONLY the
money region + checksum, proving the tool can cleanly isolate a one-action
delta when the user supplies real before/after saves.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import List, Optional, Tuple

try:
    from . import memcard as mc
    from .save import (
        SLOT_OFFSETS,
        SLOT_SIZE,
    F_PARTNER,
    F_MONEY,
    F_HOURS,
    F_MINUTES,
    F_SECONDS,
    F_PARTY_IDS,
    F_PARTY_LEVELS,
    DMW3Save,
)
except ImportError:  # run as a standalone script: `python diff.py a.gme b.gme`
    import pathlib as _pl
    import sys as _sys

    _sys.path.insert(0, str(_pl.Path(__file__).resolve().parents[2]))
    from dmw3editor.core import memcard as mc  # noqa: E402
    from dmw3editor.core.save import (  # noqa: E402
        SLOT_OFFSETS,
        SLOT_SIZE,
        F_PARTNER,
        F_MONEY,
        F_HOURS,
        F_MINUTES,
        F_SECONDS,
        F_PARTY_IDS,
        F_PARTY_LEVELS,
        DMW3Save,
    )

PAYLOAD_BASE = 0x0200
RECORD_SIZE = SLOT_SIZE * 3

# Known, verified field ranges (payload-absolute), DERIVED from the save model
# constants so this can never silently drift from the model. (start, end_excl, label)
_KNOWN: List[Tuple[int, int, str]] = [
    # The payload header is GLOBAL: one checksum/version/tag at 0x0200, before
    # slot 0. There is NOT a per-slot header.
    (PAYLOAD_BASE + 0x0000, PAYLOAD_BASE + 0x0002, "payload.checksum (u16)"),
    (PAYLOAD_BASE + 0x0002, PAYLOAD_BASE + 0x0004, "payload.version (u16)"),
    (PAYLOAD_BASE + 0x0004, PAYLOAD_BASE + 0x000C, "payload.tag 'DMW3'"),
]


def _add(base: int, rel: int, size: int, label: str) -> None:
    _KNOWN.append((base + rel, base + rel + size, label))


for _i, _slot in enumerate(SLOT_OFFSETS):
    _add(_slot, F_PARTNER, 4, f"slot{_i + 1}.partner (u32)")
    _add(_slot, F_MONEY, 4, f"slot{_i + 1}.money (u32)")
    _add(_slot, F_HOURS, 2, f"slot{_i + 1}.play_hours (u16)")
    _add(_slot, F_MINUTES, 2, f"slot{_i + 1}.play_minutes (u16)")
    _add(_slot, F_SECONDS, 2, f"slot{_i + 1}.play_seconds (u16)")
    for _p, _off in enumerate(F_PARTY_IDS):
        _add(_slot, _off, 4, f"slot{_i + 1}.party[{_p}].id (u32)")
    for _p, _off in enumerate(F_PARTY_LEVELS):
        _add(_slot, _off, 2, f"slot{_i + 1}.party[{_p}].level (u16)")

KNOWN_RANGES: List[Tuple[int, int, str]] = _KNOWN


@dataclass
class FieldDelta:
    """One contiguous range of changed bytes."""

    start: int            # payload-absolute offset
    end: int              # exclusive
    known_as: Optional[str]  # label if inside a known field, else None
    before: bytes = b""
    after: bytes = b""

    @property
    def length(self) -> int:
        return self.end - self.start

    @property
    def is_known(self) -> bool:
        return self.known_as is not None


@dataclass
class SlotDiff:
    slot: int
    deltas: List[FieldDelta] = field(default_factory=list)

    @property
    def known(self) -> List[FieldDelta]:
        return [d for d in self.deltas if d.is_known]

    @property
    def unknown(self) -> List[FieldDelta]:
        return [d for d in self.deltas if not d.is_known]

    @property
    def total_changed(self) -> int:
        return sum(d.length for d in self.deltas)


def _classify(offset: int) -> Optional[str]:
    for s, e, label in KNOWN_RANGES:
        if s <= offset < e:
            return label
    return None


def _slot_for_offset(off: int) -> int:
    """-1 means the global payload header at 0x0200; else 0/1/2 for a slot frame."""
    if PAYLOAD_BASE <= off < SLOT_OFFSETS[0]:
        return -1  # header (checksum/version/tag)
    for idx, base in enumerate(SLOT_OFFSETS):
        if base <= off < base + SLOT_SIZE:
            return idx
    return -2  # outside any known slot frame


def diff_payloads(before: bytes, after: bytes) -> List[SlotDiff]:
    """Diff two DMW3 payloads. Returns one SlotDiff per in-game save slot,
    plus a synthetic SlotDiff with slot=-1 for the global payload header."""
    if len(before) != len(after):
        raise ValueError("payloads must be the same length")
    changed = [i for i in range(len(before)) if before[i] != after[i]]
    # group consecutive changed offsets (allow 1-byte gaps as one region)
    deltas: List[FieldDelta] = []
    if changed:
        run_start = changed[0]
        run_prev = changed[0]
        for off in changed[1:]:
            if off - run_prev <= 2:
                run_prev = off
            else:
                deltas.append(_make_delta(before, after, run_start, run_prev + 1))
                run_start = off
                run_prev = off
        deltas.append(_make_delta(before, after, run_start, run_prev + 1))

    results: List[SlotDiff] = []
    for slot in (-1, 0, 1, 2):
        slot_deltas = [d for d in deltas if _slot_for_offset(d.start) == slot]
        results.append(SlotDiff(slot=slot, deltas=slot_deltas))
    return results


def _make_delta(before: bytes, after: bytes, start: int, end: int) -> FieldDelta:
    # if the region spans multiple known/unknown, pick the dominant label
    label = _classify(start)
    return FieldDelta(
        start=start,
        end=end,
        known_as=label,
        before=before[start:end],
        after=after[start:end],
    )


def diff_files(path_before: str, path_after: str) -> List[SlotDiff]:
    """Diff two memory-card images (any supported container)."""
    cb = mc.MemoryCard.load(path_before)
    ca = mc.MemoryCard.load(path_after)
    pb = cb.extract_payload(cb.find_dmw3_save())
    pa = ca.extract_payload(ca.find_dmw3_save())
    return diff_payloads(pb, pa)


def diff_card_bytes(card_before: bytes, card_after: bytes) -> List[SlotDiff]:
    """Diff two raw card bodies (131072 bytes) directly."""
    cb = mc.MemoryCard(card_before)
    ca = mc.MemoryCard(card_after)
    pb = cb.extract_payload(cb.find_dmw3_save())
    pa = ca.extract_payload(ca.find_dmw3_save())
    return diff_payloads(pb, pa)


def format_report(diff: List[SlotDiff]) -> str:
    lines: List[str] = []
    for sd in diff:
        title = "Payload header (checksum/version)" if sd.slot == -1 else f"Save slot {sd.slot + 1}"
        lines.append(f"=== {title} ===")
        if not sd.deltas:
            lines.append("  (no changes)")
            continue
        lines.append(f"  {sd.total_changed} byte(s) changed")
        for d in sd.deltas:
            tag = d.known_as if d.is_known else "UNKNOWN"
            lines.append(
                f"  0x{d.start:04X}-0x{d.end:04X} [{tag}] "
                f"was {d.before.hex(' ')} -> {d.after.hex(' ')}"
            )
        if sd.unknown:
            lines.append(
                "  ^ UNKNOWN regions are the candidate fields. Inspect in-game to "
                "confirm (e.g. if the change was 'use 1 Potion', an unknown region "
                "here is the Potion count)."
            )
    return "\n".join(lines)


def _cli() -> None:
    import argparse
    import sys as _sys

    # Allow running as a script: `python diff.py a.gme b.gme`
    if __package__ in (None, ""):
        import pathlib as _pl

        _root = _pl.Path(__file__).resolve().parents[2]
        _sys.path.insert(0, str(_root))

    ap = argparse.ArgumentParser(
        description="Diff two DMW3 saves (any container) to locate changed fields."
    )
    ap.add_argument("before", help="before save (.gme/.vgs/.mcr/.bin)")
    ap.add_argument("after", help="after save (.gme/.vgs/.mcr/.bin)")
    args = ap.parse_args()
    try:
        rep = format_report(diff_files(args.before, args.after))
    except Exception as exc:  # noqa: BLE001
        print(f"error: {exc}", file=_sys.stderr)
        _sys.exit(1)
    print(rep)


if __name__ == "__main__":
    _cli()
