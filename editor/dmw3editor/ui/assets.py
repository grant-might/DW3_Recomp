"""Runtime asset resolution for the DMW3 save editor.

Assets live under ``dmw3editor/assets``:
  roster/<Name>.png          -- 8 base-rookie avatars (front-facing idle frame,
                                extracted from the user's DW3 Assets webp
                                strips; transparent, tight-cropped).
  cards/<color>/NNN.png      -- DW3 card fronts sliced from The Spriters
                                Resource color sheets (metaldodomon rip).

All lookups are safe: missing files return None so the UI never breaks when
assets were not shipped.
"""

from __future__ import annotations

import json
import pathlib

from PySide6.QtCore import Qt
from PySide6.QtGui import QPixmap

from dmw3editor.paths import pkg_dir

ASSET_DIR = pkg_dir() / "assets"
DATA_DIR = pkg_dir() / "data"

# The 8 base rookies in DIGI_ROSTER_NAMES order.
ROSTER_NAMES = (
    "Kotemon", "Kumamon", "Monmon", "Agumon",
    "Veemon", "Guilmon", "Renamon", "Patamon",
)

# Card color families (filename stem order used by the rips).
CARD_COLORS = ("black", "blue", "brown", "green", "option", "other", "red", "white")

# save index -> {"color": str, "cell": int} (see data/card_images.json).
_card_manifest: dict[int, dict] | None = None


def _load_card_manifest() -> dict[int, dict]:
    global _card_manifest
    if _card_manifest is None:
        p = DATA_DIR / "card_images.json"
        if p.exists():
            raw = json.loads(p.read_text(encoding="utf-8"))
            _card_manifest = {
                int(k): v for k, v in raw["ids"].items()
            }
        else:
            _card_manifest = {}
    return _card_manifest


def roster_pixmap(name: str) -> QPixmap | None:
    """Return the avatar pixmap for a roster digimon name, or None."""
    p = ASSET_DIR / "roster" / f"{name}.png"
    if not p.exists():
        return None
    return QPixmap(str(p))


def card_pixmap(color: str, index: int) -> QPixmap | None:
    """Return one sliced card front, or None if missing/out of range."""
    p = ASSET_DIR / "cards" / color / f"{index:03d}.png"
    if not p.exists():
        return None
    return QPixmap(str(p))


def card_image_for_save_index(save_index: int) -> QPixmap | None:
    """Card art for a save collection index (0..313), or None if unmapped."""
    m = _load_card_manifest().get(save_index)
    if not m:
        return None
    return card_pixmap(m["color"], m["cell"])


# item-slot -> icon filename (see data/item_icons.json, user-verified map).
_item_icon_manifest: dict[int, str] | None = None


def _load_item_icon_manifest() -> dict[int, str]:
    global _item_icon_manifest
    if _item_icon_manifest is None:
        p = DATA_DIR / "item_icons.json"
        if p.exists():
            raw = json.loads(p.read_text(encoding="utf-8"))
            _item_icon_manifest = {
                int(k): v for k, v in raw["ids"].items()
            }
        else:
            _item_icon_manifest = {}
    return _item_icon_manifest


def item_icon_for_slot(slot: int) -> QPixmap | None:
    """Icon art for an item-qty slot (0..351), or None if unmapped."""
    name = _load_item_icon_manifest().get(slot)
    if not name:
        return None
    p = ASSET_DIR / "icons" / f"{name}.png"
    if not p.exists():
        return None
    return QPixmap(str(p))


def icon_pixmap(name: str) -> QPixmap | None:
    """Icon by base filename (no .png) from assets/icons."""
    p = ASSET_DIR / "icons" / f"{name}.png"
    if not p.exists():
        return None
    return QPixmap(str(p))


def card_color_count(color: str) -> int:
    """Number of card images sliced for a color family."""
    d = ASSET_DIR / "cards" / color
    if not d.exists():
        return 0
    return len([f for f in d.iterdir() if f.suffix.lower() == ".png"])


def scaled(pix: QPixmap, size: int) -> QPixmap:
    """Pixel-art friendly scale keeping aspect ratio (nearest neighbor)."""
    return pix.scaled(
        size,
        size,
        Qt.KeepAspectRatio,
        Qt.FastTransformation,
    )
