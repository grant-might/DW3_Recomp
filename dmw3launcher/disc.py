"""Disc intake: read the player's own disc image and route it to the right regional build.

The launcher ships no game code, so this module reads the disc's OWN boot data and names its region
(SLES*/SCES* = PAL Europe, SLUS*/SCUS* = NTSC-U USA). Both regions are first-class: nothing here
refuses a disc for its region. The check is a plausibility readout, never a gate on what the player
may build; a .iso or a .cue that points at a missing .bin is called out because it cannot work, and
everything else is handed to the recompiler, which is the real judge.
"""
from __future__ import annotations

import pathlib
import re
from dataclasses import dataclass

REGION_EU = "EU"
REGION_US = "US"
REGION_JP = "JP"
REGION_UNKNOWN = "?"


@dataclass
class DiscCheck:
    ok: bool
    region: str
    message: str

    @property
    def lines(self) -> list[str]:
        """The readout as the Activity log shows it: one line per non-empty line of the message."""
        return [line.strip() for line in (self.message or "").splitlines() if line.strip()]


# The disc's own serial as it sits in its boot data: SLES_039.36, SLUS-01436, SCES_123.45. The
# first occurrence in the image's early sectors is the disc's primary id (later copies are the
# boot-version list), so the router reads the head of the file and takes the first hit. This is the
# signal both the region ROUTER and the builder use, and it works without any external tool.
_IMAGE_SERIAL = re.compile(r"(SLES|SCES|SLUS|SCUS|SLPS|SCPS|SLPM|SIPS)[ _-]?\d{3}[.\-]?\d{2}", re.I)
_SERIAL_HEAD = 64 * 1024

_PREFIX_REGION = {
    "SLES": REGION_EU, "BESLES": REGION_EU, "SCES": REGION_EU, "BESCES": REGION_EU,
    "SLUS": REGION_US, "BASLUS": REGION_US, "SCUS": REGION_US, "BASCUS": REGION_US,
    "SLPS": REGION_JP, "BISLPS": REGION_JP, "SCPS": REGION_JP, "BISCPC": REGION_JP,
    "SLPM": REGION_JP, "SIPS": REGION_JP,
}


def _region_of_prefix(prefix: str) -> str:
    return _PREFIX_REGION.get(prefix.upper().strip("_- "), REGION_UNKNOWN)


def _image_bin(image: pathlib.Path) -> pathlib.Path | None:
    """The raw .bin behind an image: itself, or the file a .cue points at."""
    if image.suffix.lower() != ".cue":
        return image
    try:
        cue = image.read_text(encoding="utf-8", errors="replace")
    except OSError:
        return None
    m = re.search(r'FILE\s+"([^"]+)"\s+BINARY', cue, re.I)
    if not m:
        return None
    return image.parent / m.group(1)


def serial_of_image(image: pathlib.Path) -> str:
    """The disc serial this image carries ('' when it has none in its first sectors)."""
    p = _image_bin(image)
    if p is None or not p.is_file():
        return ""
    try:
        with open(p, "rb") as fh:
            head = fh.read(_SERIAL_HEAD)
    except OSError:
        return ""
    m = _IMAGE_SERIAL.search(head.decode("latin-1"))
    return m.group(0).upper() if m else ""


def region_of_image(image: pathlib.Path) -> str:
    """Classify an image by its own serial: SLES*/SCES* = PAL, SLUS*/SCUS* = NTSC-U.

    Returns REGION_UNKNOWN when there is no serial to read; nothing is refused on that basis, it
    only means the router cannot name the region up front.
    """
    serial = serial_of_image(image)
    if not serial:
        return REGION_UNKNOWN
    return _PREFIX_REGION.get(serial[:4], REGION_UNKNOWN)


def region_of_serial(serial: str) -> str:
    """Map a bare licence code ('SLES_039.36', 'SLUS-01436') to a region, or UNKNOWN."""
    s = (serial or "").strip().upper()
    for prefix, region in _PREFIX_REGION.items():
        if s.startswith(prefix):
            return region
    return REGION_UNKNOWN


def verify(image: pathlib.Path) -> DiscCheck:
    """Judge the player's own file before anything expensive happens.

    The checks the player's own file needs come first, because they are the ones we can answer
    without a tool and the ones whose message is actually actionable. A file that passes is not
    promised to build, only handed to the recompiler; the region is read from the disc's own serial.
    """
    if not image.is_file():
        return DiscCheck(False, REGION_UNKNOWN, f"Disc image not found: {image}")

    suffix = image.suffix.lower()
    if suffix == ".iso":
        return DiscCheck(False, REGION_UNKNOWN,
                         "A .iso will not work: the movies are stored as raw sectors. Use the "
                         ".cue + 2352-byte-per-sector .bin that came off your disc.")
    if suffix == ".cue":
        try:
            cue = image.read_text(encoding="utf-8", errors="replace")
        except OSError as exc:
            return DiscCheck(False, REGION_UNKNOWN, f"Could not read {image.name}: {exc}")
        if ".bin" not in cue.lower():
            return DiscCheck(False, REGION_UNKNOWN,
                             "That .cue does not point at a .bin. It needs the raw 2352-byte-"
                             "per-sector image that sits next to it.")
        target = _image_bin(image)
        if target is None or not target.is_file():
            name = target.name if target is not None else "its .bin"
            return DiscCheck(False, REGION_UNKNOWN,
                             f'That .cue points at "{name}", but no such file is next to it.')
    elif suffix != ".bin":
        return DiscCheck(False, REGION_UNKNOWN,
                         f"Unsupported file type '{suffix or image.name}'. Give me the .cue (or "
                         f"the .bin) from your disc image.")

    serial = serial_of_image(image)
    region = region_of_serial(serial)
    if region == REGION_UNKNOWN:
        return DiscCheck(True, REGION_UNKNOWN,
                         "This image carries no PAL or NTSC-U serial in its first sectors. It can "
                         "still be built; the recompiler is the judge. Pick the .bin/.cue that came "
                         "off your own Digimon World 3 disc if you meant to.")
    return DiscCheck(True, region, f"Readable raw disc image; its serial ({serial}) is a {region} disc.")


def candidates_in(folder: pathlib.Path) -> list[pathlib.Path]:
    """Disc images the player has dropped in a folder."""
    if not folder.is_dir():
        return []
    found = []
    for pat in ("*.cue", "*.bin"):
        found.extend(sorted(folder.glob(pat)))
    return found


def describe_disc_name(image: pathlib.Path) -> str:
    """Readable label for the disc list, without probing the image."""
    stem = re.sub(r"[_]+", " ", image.stem)
    return stem
