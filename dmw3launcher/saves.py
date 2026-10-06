"""Finding the player's saves, so the Memory Card tab can open them without a file dialog.

Two shapes exist in the wild:
  * the runtime's own slots — <root>/DMW3Game/SAVEDATA/save0..3.sav, each a bare 10060-byte
    DMW3 payload (game.toml points memcard_dir there);
  * real PS1 memory cards (.mcr/.mcd/.mc, 128 KiB) with a BASLUS/BESLES/BISLPS entry inside.
"""
from __future__ import annotations

import pathlib
from dataclasses import dataclass

from . import paths

SLOT_SUFFIXES = (".sav",)
CARD_SUFFIXES = (".mcr", ".mcd", ".mc", ".mcs", ".ps1")


@dataclass
class Found:
    kind: str          # "slot" | "card"
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


def discover(root: pathlib.Path, extra_card_dirs: list[pathlib.Path] | None = None) -> list[Found]:
    """Every save we can find for this install, newest first."""
    out: list[Found] = []

    sd = paths.savedata_dir(root)
    if sd.is_dir():
        for p in sorted(sd.iterdir()):
            if p.suffix.lower() in SLOT_SUFFIXES and p.is_file():
                size, mtime = _stat(p)
                out.append(Found("slot", p, f"{p.stem}  (game slot)", size, mtime))

    dirs = [paths.discs_dir(root), paths.bin_dir(root), root / "cards",
            paths.user_data_dir() / "cards"]
    dirs.extend(extra_card_dirs or [])
    seen: set[pathlib.Path] = set()
    for d in dirs:
        if not d.is_dir():
            continue
        for p in sorted(d.iterdir()):
            if p.suffix.lower() in CARD_SUFFIXES and p.is_file() and p not in seen:
                seen.add(p)
                size, mtime = _stat(p)
                out.append(Found("card", p, f"{p.name}  (memory card)", size, mtime))

    out.sort(key=lambda f: f.mtime, reverse=True)
    return out


def default_open(found: Found):
    """Return (payload_or_card_path, is_card) — what the editor needs to open it.

    Slot files are bare payloads, so they bypass the card container entirely.
    """
    return (found.path, found.is_card)
