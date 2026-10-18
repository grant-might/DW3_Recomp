"""Finding the player's memory cards, so the Memory Card tab can open them without a file dialog.

The launcher's builds keep their saves on real PS1 memory cards: each regional build streams
`card1.mcd` / `card2.mcd` (standard 128 KiB PSX cards) from its own folder, and players often keep
extra cards beside the launcher. This module collects every card it can find so the embedded editor
can open one directly.

All three releases are first-class, the Japanese one included: `Builds/JP/` is searched exactly like
`Builds/USA/` and `Builds/EUR/`, and a card's own product code names its region through `disc.py`'s
one table - the launcher keeps no second list of codes to drift from it.
"""
from __future__ import annotations

import pathlib
from dataclasses import dataclass

from . import builds, disc, paths

CARD_SUFFIXES = (".mcr", ".mcd", ".mc", ".mcs", ".ps1")


def entry_region(name: str) -> str:
    """The region a card's directory entry belongs to (disc.REGION_US / EU / JP / UNKNOWN).

    A memory card names its save after the disc's OWN product code - `BASLUS-01436DMW3-USA`,
    `BESLES-03936DMW3-EUR`, `BISLPS-03446DMW3-JPN` - so the same table that classifies a disc
    classifies a card. Japan therefore needs no special case anywhere in the launcher.
    """
    return disc.region_of_serial(name)


def region_label(region: str) -> str:
    """The launcher's own name for a region: the label the Play button for it carries.

    One vocabulary for the whole app (USA / Europe / Japan), and it comes from the build list rather
    than from a second set of names kept here.
    """
    if builds.known_region(region):
        return builds.spec(region).label
    return "unrecognised region"


def entry_label(name: str) -> str:
    """How a card's Digimon World 3 save reads in the UI, e.g. "Digimon World 3 (Japan)"."""
    return f"Digimon World 3 ({region_label(entry_region(name))})"


@dataclass
class Found:
    kind: str          # "card"
    path: pathlib.Path
    label: str
    size: int
    mtime: float

    @property
    def is_card(self) -> bool:
        return self.kind == "card"


def _stat(p: pathlib.Path) -> tuple[int, float]:
    try:
        st = p.stat()
        return st.st_size, st.st_mtime
    except OSError:
        return 0, 0.0


def search_dirs(extra_card_dirs: list[pathlib.Path] | None = None) -> list[pathlib.Path]:
    """Every folder a memory card may live in, most specific first."""
    dirs = list(builds.card_dirs())                 # cards/USA, cards/EUR, cards/JP (card1/card2)
    dirs += [paths.cards_dir(), paths.user_data_dir() / "cards"]
    dirs += list(extra_card_dirs or [])
    return dirs


def discover(extra_card_dirs: list[pathlib.Path] | None = None) -> list[Found]:
    """Every memory card we can find, newest first."""
    out: list[Found] = []
    seen: set[pathlib.Path] = set()
    for d in search_dirs(extra_card_dirs):
        if not d.is_dir():
            continue
        try:
            entries = sorted(d.iterdir())
        except OSError:
            continue
        for p in entries:
            if (p.suffix.lower() in CARD_SUFFIXES and p.is_file()
                    and p.resolve() not in seen):
                seen.add(p.resolve())
                size, mtime = _stat(p)
                out.append(Found("card", p, f"{p.name}  ({p.parent.name} memory card)",
                                 size, mtime))
    out.sort(key=lambda f: f.mtime, reverse=True)
    return out


def default_open(found: Found):
    """Return (card_path, is_card) - what the editor needs to open it."""
    return (found.path, found.is_card)
