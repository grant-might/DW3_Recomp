r"""Generate placeholder art for every asset slot the launcher actually loads.

Placeholders are deliberately tasteful (launcher palette, dashed accent border, the filename and
size printed on the image) so the app looks intentional as a wireframe and you can see exactly
which slot each file fills. Replace them one at a time; nothing breaks in between.

    .\.venv\Scripts\python.exe make_placeholders.py

Writes into assets/. Existing files are only overwritten if --force is given.
"""
from __future__ import annotations

import pathlib
import struct
import sys

from PySide6.QtCore import QBuffer, QByteArray, QRect, Qt
from PySide6.QtGui import QColor, QFont, QIcon, QImage, QPainter, QPen, QPixmap
from PySide6.QtWidgets import QApplication

ROOT = pathlib.Path(__file__).resolve().parent
ASSETS = ROOT / "assets"

BG = QColor("#1e2230")
BG2 = QColor("#171a24")
ACCENT = QColor("#f2a33c")
TEXT = QColor("#e8ecf7")
DIM = QColor("#9aa3bd")

# name -> (width, height, label)
# `background.png` and `header_bar.png` are deliberately NOT here: those two slots are art-less by
# the user's decision (the window paints the live theme colour and the header a flat panel), and a
# generator that quietly recreates them would put a picture back over that choice.
SHEETS = {
    "logo.png":          (512, 512, "logo.png\n512 x 512  (2x: 1024 x 1024)\nor 640 x 160 wordmark"),
    "play_banner.png":   (1150, 220, "play_banner.png\n1150 x 220  (2x: 2300 x 440)"),
    "tab_play.png":      (32, 32, "P"),
    "tab_memcard.png":   (32, 32, "C"),
    "tab_mods.png":      (32, 32, "M"),
    "tab_decomp.png":    (32, 32, "D"),
    "tab_settings.png":  (32, 32, "S"),
    "icon.png":          (256, 256, "DW3"),
}


def make_image(w: int, h: int, label: str, icon_mode: bool = False) -> QImage:
    img = QImage(w, h, QImage.Format.Format_ARGB32)
    img.fill(Qt.GlobalColor.transparent)
    p = QPainter(img)
    p.setRenderHint(QPainter.RenderHint.Antialiasing)
    p.setRenderHint(QPainter.RenderHint.TextAntialiasing)

    radius = max(3, min(w, h) // 12)
    p.setPen(Qt.PenStyle.NoPen)
    p.setBrush(BG2)
    p.drawRoundedRect(0, 0, w, h, radius, radius)

    # dashed accent border = "this is a placeholder"
    pen = QPen(ACCENT)
    pen.setWidth(max(1, min(w, h) // 90))
    pen.setStyle(Qt.PenStyle.DashLine)
    p.setPen(pen)
    p.setBrush(Qt.BrushStyle.NoBrush)
    inset = pen.width()
    p.drawRoundedRect(inset, inset, w - 2 * inset, h - 2 * inset, radius, radius)

    # a faint diagonal cross so it reads as filler at a glance
    pen2 = QPen(QColor(ACCENT.red(), ACCENT.green(), ACCENT.blue(), 40))
    pen2.setWidth(1)
    p.setPen(pen2)
    p.drawLine(0, 0, w, h)
    p.drawLine(w, 0, 0, h)

    p.setPen(TEXT)
    f = QFont("Segoe UI")
    if icon_mode or max(w, h) <= 64:
        f.setPointSize(max(7, min(w, h) // 3 if not icon_mode else min(w, h) // 4))
        f.setBold(True)
    else:
        f.setPointSize(max(9, min(w, h) // 14))
    p.setFont(f)
    p.drawText(QRect(0, 0, w, h), Qt.AlignmentFlag.AlignCenter, label)
    p.end()
    return img


def png_bytes(img: QImage) -> bytes:
    ba = QByteArray()
    buf = QBuffer(ba)
    buf.open(QBuffer.OpenModeFlag.WriteOnly)
    img.save(buf, "PNG")
    return bytes(ba)


def write_ico(path: pathlib.Path, sizes: list[int]) -> None:
    """A multi-size .ico with PNG payloads (Windows Vista+ reads these).

    Deliberately hand-rolled: Qt cannot write multi-size ICO, and this is the one asset whose
    whole point is containing several sizes.
    """
    entries = []
    for s in sizes:
        img = make_image(s, s, "DW3", icon_mode=True)
        entries.append((s, png_bytes(img)))

    header = struct.pack("<HHH", 0, 1, len(entries))
    offset = len(header) + 16 * len(entries)
    directory = b""
    for s, data in entries:
        dim = 0 if s >= 256 else s
        directory += struct.pack("<BBBBHHII", dim, dim, 0, 0, 1, 32, len(data), offset)
        offset += len(data)
    path.write_bytes(header + directory + b"".join(d for _, d in entries))


def main() -> int:
    app = QApplication(sys.argv)  # noqa: F841 (needed for QImage/QPainter)
    ASSETS.mkdir(parents=True, exist_ok=True)
    force = "--force" in sys.argv
    made, kept = [], []

    for name, (w, h, label) in SHEETS.items():
        p = ASSETS / name
        if p.exists() and not force:
            kept.append(name)
            continue
        img = make_image(w, h, label, icon_mode=(name == "icon.png"))
        img.save(str(p), "PNG")
        made.append(f"{name} {w}x{h}")

    ico = ASSETS / "icon.ico"
    if not ico.exists() or force:
        write_ico(ico, [16, 24, 32, 48, 64, 128, 256])
        made.append("icon.ico 16/24/32/48/64/128/256")

    print("created :", ", ".join(made) if made else "(none)")
    if kept:
        print("kept    :", ", ".join(kept), "(already existed; use --force to regenerate)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
