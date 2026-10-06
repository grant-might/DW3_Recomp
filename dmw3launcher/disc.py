"""Disc intake: read the player's own disc image and route it to the right regional build.

The launcher ships no game code, so this module's first job is to read the disc's OWN boot data
and name its region (SLES*/SCES* = PAL Europe, SLUS*/SCUS* = NTSC-U USA). Both regions are
first-class: nothing here refuses a USA disc. DiscTool is still understood, but only as an
optional completeness readout when a copy of it happens to sit beside the launcher, never as a
gate on what the player may build or launch.
"""
from __future__ import annotations

import pathlib
import re
import subprocess
from dataclasses import dataclass

from dmw3launcher import tooltext

REGION_EU = "EU"
REGION_US = "US"
REGION_JP = "JP"
REGION_UNKNOWN = "?"


@dataclass
class DiscCheck:
    ok: bool
    region: str
    message: str
    raw: str = ""

    @property
    def lines(self) -> list[str]:
        """The tool's own report in English, one entry per non-empty line: what the log shows.

        Lives here, not in the UI, so a screen cannot show the Spanish report by accident.
        """
        return [line.strip() for line in tooltext.english(self.raw).splitlines() if line.strip()]


def _run(tool: pathlib.Path, args: list[str], cwd: pathlib.Path | None = None,
         timeout: int = 120) -> subprocess.CompletedProcess:
    return subprocess.run(
        [str(tool), *args],
        cwd=str(cwd) if cwd else None,
        capture_output=True,
        text=True,
        errors="replace",
        timeout=timeout,
    )


def tool_available(tool: pathlib.Path) -> bool:
    return tool.is_file()


# The disc's own boot code is the only trustworthy signal. DiscTool's refusal text names BOTH
# regions ("its disc boots with SLUS-01436 ... the European version (SLES-03936) is required"),
# so scanning the whole report for a licence code classifies a USA disc as EU.
_EXEC_LINE = re.compile(r"ejecutable\s*:\s*([A-Z]{4})[_\- ]?(\d{3})[.\-]?(\d{2})", re.I)
_BOOTS_WITH = re.compile(r"arranca con\s+([A-Z]{4})[_\- ]?(\d{3})[.\-]?(\d{2})", re.I)
_PREFIX_REGION = {
    "SLES": REGION_EU, "BESLES": REGION_EU, "SCES": REGION_EU, "BESCES": REGION_EU,
    "SLUS": REGION_US, "BASLUS": REGION_US, "SCUS": REGION_US, "BASCUS": REGION_US,
    "SLPS": REGION_JP, "BISLPS": REGION_JP, "SCPS": REGION_JP, "BISCPC": REGION_JP,
    "SLPM": REGION_JP, "SIPS": REGION_JP,
}


def _region_of_prefix(prefix: str) -> str:
    return _PREFIX_REGION.get(prefix.upper().strip("_- "), REGION_UNKNOWN)


# The disc's own serial as it sits in its boot data: SLES_039.36, SLUS-01436, SCES_123.45. The
# first occurrence in the image's early sectors is the disc's primary id (later copies are the
# boot-version list), so the router reads the head of the file and takes the first hit. This is
# the signal both the region ROUTER and the builder use (the launcher ships no DiscTool), so it
# has to work without any external tool.
_IMAGE_SERIAL = re.compile(r"(SLES|SCES|SLUS|SCUS|SLPS|SCPS|SLPM|SIPS)[ _-]?\d{3}[.\-]?\d{2}",
                           re.I)
_SERIAL_HEAD = 64 * 1024


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

    Returns REGION_UNKNOWN when there is no serial to read; nothing is refused on that basis,
    it only means the router cannot name the region up front.
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


def region_from_output(text: str) -> str:
    t = text.upper()
    for pattern in (_EXEC_LINE, _BOOTS_WITH):   # what the disc says about itself, first
        m = pattern.search(t)
        if m and "NINGUNO" not in m.group(0):
            region = _region_of_prefix(m.group(1))
            if region != REGION_UNKNOWN:
                return region
    # Fall back to the licence codes this port knows. USA and Japan are tested before the bare
    # "Europe" words, which any edition's refusal text is happy to mention.
    if "SLUS-01436" in t or "BASLUS-01436" in t:
        return REGION_US
    if "SLPS-03050" in t or "BISLPS-03050" in t:
        return REGION_JP
    if "SLES-03936" in t or "BESLES-03936" in t or "EUROPEA" in t or "EUROPE" in t:
        return REGION_EU
    return REGION_UNKNOWN


def verify(tool: pathlib.Path, image: pathlib.Path) -> DiscCheck:
    """`DiscTool ver <imagen>` — the project's own validation, including completeness.

    The checks the player's own file needs come first: they are the ones we can answer
    without the tool, and they are the ones whose message is actually actionable.
    """
    if not image.is_file():
        return DiscCheck(False, REGION_UNKNOWN, f"Disc image not found: {image}")

    suffix = image.suffix.lower()
    if suffix == ".iso":
        return DiscCheck(False, REGION_UNKNOWN,
                         "A .iso will not work: the movies are stored as raw sectors. Use the "
                         ".cue + 2352-byte-per-sector .bin that came off your disc.")
    if suffix == ".cue":
        cue = image.read_text(encoding="utf-8", errors="replace")
        if ".bin" not in cue.lower():
            return DiscCheck(False, REGION_UNKNOWN,
                             "That .cue does not point at a .bin. It needs the raw 2352-byte-"
                             "per-sector image that sits next to it.")
    elif suffix != ".bin":
        return DiscCheck(False, REGION_UNKNOWN,
                         f"Unsupported file type '{suffix or image.name}'. Give me the .cue (or "
                         f"the .bin) from your disc image.")

    if not tool_available(tool):
        return DiscCheck(False, REGION_UNKNOWN,
                         f"DiscTool.exe not found at {tool}. Point the launcher at the "
                         f"recomp project folder that contains it.")

    try:
        res = _run(tool, ["ver", str(image)])
    except subprocess.TimeoutExpired:
        return DiscCheck(False, REGION_UNKNOWN, "DiscTool timed out reading the image.")

    out = (res.stdout or "") + (res.stderr or "")
    region = region_from_output(out)
    ok = res.returncode == 0 and "ES EL DISCO CORRECTO" in out.upper()

    if ok:
        return DiscCheck(True, REGION_EU, "Valid Digimon World 2003 disc.", out)

    # DiscTool is chatty about WHY it refused. Keep its own words, but in English — the tool
    # speaks Spanish and this UI does not. The untranslated text stays in .raw for bug reports.
    why = tooltext.english(_first_meaningful(out)) or f"DiscTool exited {res.returncode}."
    if region == REGION_US:
        why = (why + "\n\nThis runtime is compiled from the European disc only, so a USA disc "
                     "cannot be installed into it. Playing USA natively needs a second "
                     "recompilation of the SLUS binary.")
    return DiscCheck(False, region, why, out)


_VERDICT = re.compile(r"^\s*veredicto\s*:\s*(.+)$", re.I | re.M)
_FAILURE = re.compile(r"^\s*fallo\s*:\s*(.+)$", re.I | re.M)


def _first_meaningful(text: str) -> str:
    """The line that explains the refusal, out of DiscTool's report.

    DiscTool states its conclusion on the `veredicto :` line (`fallo :` for the other
    subcommands); the `volumen :` / `ejecutable :` lines above it are context, not the reason.
    """
    for pattern in (_VERDICT, _FAILURE):
        m = pattern.search(text or "")
        if m and len(m.group(1).strip()) > 8:
            return m.group(1).strip()
    lines = [l.strip() for l in (text or "").splitlines() if l.strip()]
    for line in lines:
        if (len(line) >= 20 and not line.startswith("DiscTool ")
                and not re.match(r"^[^\s:]{1,16}\s*:", line)):
            return line
    return ""


def extract(tool: pathlib.Path, image: pathlib.Path, install_root: pathlib.Path,
            log: callable | None = None) -> tuple[bool, str]:
    """`DiscTool sacar <imagen> <destino>` — build the loose file tree the runtime plays from.

    `log` receives progress lines if given.
    """
    if not tool_available(tool):
        return False, f"DiscTool.exe not found at {tool}."
    install_root.mkdir(parents=True, exist_ok=True)
    try:
        res = _run(tool, ["sacar", str(image), str(install_root)], timeout=3600)
    except subprocess.TimeoutExpired:
        return False, "Extraction timed out."
    out = (res.stdout or "") + (res.stderr or "")
    if log:
        for line in out.splitlines():
            log(tooltext.english(line))      # the activity log is player-facing too
    return res.returncode == 0, out


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
