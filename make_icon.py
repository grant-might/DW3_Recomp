"""Build assets/icon.ico from the supplied icon art.

Windows wants the small sizes as their own frames; a single scaled frame looks soft in the taskbar
and in Explorer's small views. This embeds one PNG per size (allowed in ICO since Vista, and exact
on the round trip) at the set the launcher expects: 16, 24, 32, 48, 64, 128 and 256.

    .\\.venv\\Scripts\\python.exe make_icon.py            # from icon.png in the project root
    .\\.venv\\Scripts\\python.exe make_icon.py --force    # rewrite even if icon.ico is newer

Source of truth is the PNG you supply (project root `icon.png`, usually 512x512). The .ico is a
derived artefact: delete it and `theme.app_icon()` falls back to the PNG, so nothing breaks.
"""
from __future__ import annotations

import pathlib
import struct
import sys

from PySide6.QtCore import QBuffer, QByteArray, Qt
from PySide6.QtGui import QImage

ROOT = pathlib.Path(__file__).resolve().parent
ASSETS = ROOT / "assets"
SIZES = (256, 128, 64, 48, 32, 24, 16)


def png_bytes(image: QImage) -> bytes:
    buffer, raw = QBuffer(), QByteArray()
    buffer.setBuffer(raw)
    buffer.open(QBuffer.OpenModeFlag.WriteOnly)
    image.save(buffer, "PNG")
    buffer.close()
    return bytes(raw)


def build(source: pathlib.Path, target: pathlib.Path, sizes: tuple[int, ...] = SIZES) -> int:
    image = QImage(str(source))
    if image.isNull():
        raise SystemExit(f"cannot read {source}")
    frames = []
    for size in sizes:
        scaled = image.scaled(size, size, Qt.AspectRatioMode.KeepAspectRatio,
                              Qt.TransformationMode.SmoothTransformation)
        frames.append((size, png_bytes(scaled)))
    header = struct.pack("<HHH", 0, 1, len(frames))          # reserved, type=1 (ICO), frame count
    offset = 6 + 16 * len(frames)
    table, blobs = b"", b""
    for size, data in frames:
        # A dimension of 256 is written as 0: the field is one byte.
        table += struct.pack("<BBBBHHII", size if size < 256 else 0, size if size < 256 else 0,
                             0, 0, 1, 32, len(data), offset)
        offset += len(data)
        blobs += data
    target.write_bytes(header + table + blobs)
    return len(frames)


def main() -> int:
    force = "--force" in sys.argv
    source = ROOT / "icon.png"
    if not source.is_file():
        # assets/icon.png is the copy the app ships; the root one is the author's master.
        source = ASSETS / "icon.png"
    if not source.is_file():
        raise SystemExit("no icon.png to build from (project root or assets/)")
    target = ASSETS / "icon.ico"
    if target.is_file() and not force and target.stat().st_mtime >= source.stat().st_mtime:
        print(f"kept    : {target.name} is newer than {source.name} (use --force to rebuild)")
        return 0
    if target.is_file() and not (ASSETS / "icon.ico.placeholder.bak").is_file():
        (ASSETS / "icon.ico.placeholder.bak").write_bytes(target.read_bytes())
    frames = build(source, target)
    print(f"built   : {target.name} from {source.name}: {frames} frames {list(SIZES)}, "
          f"{target.stat().st_size} bytes")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
