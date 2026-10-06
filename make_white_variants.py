"""Bake white-text copies of the tab art for the dark chrome, leaving the icons untouched.

The supplied tab art draws its word in near-black, which is invisible on this launcher's dark
themes, and PLAY/MODS also carry a coloured pixel icon. Repainting the black ink at load time is
what damaged the MODS icon: the icon's outline and centre are dark too, so they got repainted with
the text.

This tool splits the art by geometry instead. It finds the horizontal runs of ink separated by
transparent gaps, and a run containing colour is treated as the icon and copied through pixel for
pixel; every other run is treated as the word and repainted white, keeping its alpha (so the
anti-aliasing survives). The result is written next to the original as `tab_<tab>_white.png`; the
original is never modified, and an existing copy is only replaced with --force.

    .\\.venv\\Scripts\\python.exe make_white_variants.py [--force]
"""
from __future__ import annotations

import pathlib
import sys

from PySide6.QtGui import QColor, QImage
from PySide6.QtWidgets import QApplication

ROOT = pathlib.Path(__file__).resolve().parent
ASSETS = ROOT / "assets"
TABS = ("play", "memcard", "mods", "decomp", "settings")
GAP = 3            # columns of transparency that separate two runs
CHROMA = 25        # max-min channel spread that counts as colour rather than grey/black
MIN_COLOURED = 8   # a run needs at least this many coloured pixels to be an icon
WHITE = QColor(255, 255, 255)


def runs(image: QImage, gap: int = GAP) -> list[tuple[int, int]]:
    """Horizontal runs of columns that contain ink, split at gaps of >= ``gap`` empty columns."""
    filled = [any(image.pixelColor(x, y).alpha() > 8 for y in range(image.height()))
              for x in range(image.width())]
    out: list[tuple[int, int]] = []
    start: int | None = None
    empty = 0
    for x, has_ink in enumerate(filled):
        if has_ink:
            if start is None:
                start = x
            empty = 0
        elif start is not None:
            empty += 1
            if empty >= gap:
                out.append((start, x - empty))
                start = None
    if start is not None:
        out.append((start, len(filled) - 1))
    return out


def coloured_count(image: QImage, a: int, b: int) -> int:
    n = 0
    for y in range(image.height()):
        for x in range(a, b + 1):
            c = image.pixelColor(x, y)
            if c.alpha() > 8:
                spread = max(c.red(), c.green(), c.blue()) - min(c.red(), c.green(), c.blue())
                if spread > CHROMA:
                    n += 1
    return n


def whiten(image: QImage, spans: list[tuple[int, int]]) -> int:
    """Repaint the ink in ``spans`` white, keeping alpha. Returns how many pixels changed."""
    changed = 0
    for a, b in spans:
        for y in range(image.height()):
            for x in range(a, b + 1):
                c = image.pixelColor(x, y)
                if c.alpha() > 8:
                    image.setPixelColor(x, y, QColor(WHITE.red(), WHITE.green(), WHITE.blue(),
                                                     c.alpha()))
                    changed += 1
    return changed


def main() -> int:
    app = QApplication(sys.argv)  # noqa: F841 (QImage/QPainter)
    force = "--force" in sys.argv
    made, kept, skipped = [], [], []

    for tab in TABS:
        src = ASSETS / f"tab_{tab}.png"
        dst = ASSETS / f"tab_{tab}_white.png"
        if not src.is_file():
            skipped.append(f"tab_{tab}.png (absent)")
            continue
        if dst.exists() and not force:
            kept.append(dst.name)
            continue

        image = QImage(str(src)).convertToFormat(QImage.Format.Format_ARGB32)
        report = []
        words: list[tuple[int, int]] = []
        for a, b in runs(image):
            if coloured_count(image, a, b) >= MIN_COLOURED:
                report.append(f"icon x{a}-{b} (kept as drawn)")
            else:
                words.append((a, b))
                report.append(f"word x{a}-{b} (white)")
        changed = whiten(image, words)
        image.save(str(dst), "PNG")
        made.append(f"{dst.name}: {', '.join(report)}; {changed} px whitened")

    print("created :")
    for line in made:
        print("   ", line)
    if kept:
        print("kept    :", ", ".join(kept), "(exist already; use --force to regenerate)")
    if skipped:
        print("skipped :", ", ".join(skipped))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
