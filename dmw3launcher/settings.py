"""Read and write the runtime's settings.toml WITHOUT destroying it.

The file is hand-editable and its comments document what every key means (and the in-game
settings menu writes it too). A plain toml round-trip would drop all of that, so every write
goes through tomlkit and only touches the keys the launcher manages.
"""
from __future__ import annotations

import pathlib
from typing import Any

try:
    import tomlkit
except ImportError:  # pragma: no cover - surfaced by the launcher with a clear message
    tomlkit = None  # type: ignore[assignment]

# section -> key -> (label, kind, options)  --- only what we can validate honestly.
# Every key exposed here is one the runtime actually reads: each was confirmed present in the exe's
# own strings, and the game logs the ones it applies at boot
# ("ajustes: renderer=opengl load_speed=4x aspect_ratio=4:3 fullscreen=0 supersampling=4
#  frame_interpolation=true mute_unfocused=true"). Keys in settings.toml that are the port's own
# business - `bios_hle`, `launcher.skip_launcher` - are deliberately NOT exposed: they are how the
# runtime starts up, not player preferences.
VIDEO_BOOLS = (
    ("antialiasing", "Anti-aliasing"),
    ("frame_interpolation", "Frame interpolation"),
)
VIDEO_CHOICES = (
    ("renderer", "Renderer", ("opengl", "vulkan", "software")),
    ("texture_filtering", "Texture filter", ("nearest", "linear")),
    ("crt_filter", "CRT filter", ("raw", "scanlines", "smooth")),
    ("aspect_ratio", "Aspect ratio", ("4:3", "16:9")),
)
VIDEO_INTS = (
    ("supersampling", "Supersampling", 0, 8),
)
# 0/1 in the file, shown as words and stored as the number.
FULLSCREEN_CHOICES = (
    ("Windowed", 0),
    ("Fullscreen", 1),
)
# `window_width` is the runtime's only size knob - it derives the height from `aspect_ratio` - so
# the launcher offers a "Window size" dropdown and writes the width. The label spells out the 4:3
# height it works out to, next to the aspect-ratio row it depends on; with another aspect ratio the
# height follows that instead.
WINDOW_SIZE_CHOICES = (
    ("1280 × 960", 1280),
    ("1440 × 1080", 1440),
    ("1600 × 1200", 1600),
    ("1920 × 1440", 1920),
    ("2560 × 1920", 2560),
    ("3840 × 2880", 3840),
)
AUDIO_BOOLS = (
    ("spu_hq", "High-quality SPU audio"),
    ("mute_unfocused", "Mute when unfocused"),
)
# These live in [video] in the file but are about getting through the game rather than picture
# quality, so the UI groups them with Loading. The section written is still "video" either way.
LOADING_BOOLS = (
    ("fast_boot", "Fast boot"),
    ("turbo_loads", "Turbo loads"),
    ("auto_skip_fmv", "Auto-skip FMV"),
)
LOAD_CHOICES = (
    ("load_speed", "Load speed", ("instant", "1x", "2x", "4x")),
)
# The author's own note on this one: 512 hung the game, 96 is the safe step, and it is the knob to
# LOWER when something wedges.
LOAD_STEPS = ("96", "64", "48", "32")
CONTROLLER_FIELDS = (
    ("p1_device", "Player 1 device"),
    ("p1_mode", "Player 1 mode"),
    ("p2_device", "Player 2 device"),
    ("p2_mode", "Player 2 mode"),
)


def available() -> bool:
    return tomlkit is not None


def load(path: pathlib.Path):
    """Parse the settings file, preserving comments and ordering."""
    text = path.read_text(encoding="utf-8-sig")
    if tomlkit is None:
        raise RuntimeError("tomlkit is not installed (pip install tomlkit)")
    return tomlkit.parse(text)


def read_value(doc, section: str, key: str, default: Any = None) -> Any:
    try:
        return doc[section][key]
    except Exception:
        return default


def set_values(path: pathlib.Path, updates: dict[tuple[str, str], Any]) -> None:
    """Apply `{(section, key): value}` and write back, keeping every comment.

    Unknown keys, unknown sections and all comments survive untouched, the file stays
    hand-editable and the game's own settings menu keeps working on it.
    """
    doc = load(path)
    for (section, key), value in updates.items():
        if section not in doc:
            doc[section] = tomlkit.table()
        doc[section][key] = value
    path.write_text(tomlkit.dumps(doc), encoding="utf-8")


def snapshot(path: pathlib.Path) -> dict[str, dict[str, Any]]:
    """Plain-dict view of the file, for the UI.

    Never raises: a missing file, a missing tomlkit or a settings.toml the player has broken
    by hand must not take the launcher down.
    """
    if not path.is_file() or tomlkit is None:
        return {}
    try:
        doc = load(path)
    except Exception:
        return {}
    out: dict[str, dict[str, Any]] = {}
    for section, table in doc.items():
        if hasattr(table, "items"):
            out[str(section)] = {str(k): v for k, v in table.items()}
    return out


def load_speed_presets(path: pathlib.Path) -> dict[str, Any]:
    """The load-speed knobs, which the runtime documents as the ones to back off when a
    zone wedges. Exposed so the launcher can offer 'safe' / 'fast' presets."""
    doc = load(path)
    return {
        "load_speed": read_value(doc, "localization", "load_speed", "4x"),
        "load_sectors_per_frame": read_value(doc, "localization", "load_sectors_per_frame", "96"),
    }
