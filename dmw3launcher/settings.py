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
    ("renderer", "Renderer", ("vulkan", "opengl", "software")),
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

# ---------------------------------------------------------------- Video & performance
# The pacing / latency / rewind knobs, split by WHERE the runtime reads them.
#
# Everything below was confirmed against the engine, not guessed:
#   * engine/framework/recompiler/src/config_loader.cpp load_user_settings() parses the [video]
#     keys `vsync`, `low_latency_input`, `frame_interpolation_fps`, `rewind_depth`,
#     `rewind_interval` from a build's own settings.toml;
#   * engine/framework/runtime/src/main.cpp (the `[video]` block seeded into the internal launcher
#     and the env parser after it) applies them. The four LATENCY knobs also have an environment
#     override that WINS over the file: PSX_VSYNC, PSX_LOW_LATENCY_INPUT, PSX_FRAME_INTERPOLATION,
#     PSX_FRAME_INTERPOLATION_FPS (main.cpp ~L11149-11162).
#   * a few presentation knobs exist ONLY as environment variables — no settings.toml key at all —
#     so those are stored in the launcher's own config and put on the launched process's
#     environment instead (see ENV_* below and builds.launch).
#
# `vsync` is a STRING in the file: "on" (tear-free) / "immediate" (lowest display latency) /
# "adaptive". The runtime's rule (main.cpp present_vsync_owns_cadence, ~L2510): driver vsync only
# clocks a ~60 Hz panel; on any other refresh the wall-clock 59.94 Hz pacer owns cadence and the
# swap interval is forced off, so a 120/144 Hz panel does NOT run the sim fast.
VSYNC_CHOICES = (
    ("Driver vsync (tear-free)", "on"),
    ("Immediate (lowest latency)", "immediate"),
    ("Adaptive", "adaptive"),
)
INTERP_FPS_CHOICES = (
    ("Display refresh", 0),
    ("90 FPS", 90),
    ("120 FPS", 120),
    ("144 FPS", 144),
)
# UI offers these exact steps; the runtime snaps any other value to the nearest one it knows.
REWIND_DEPTH_CHOICES = (
    ("50 snaps (~12.5 s)", 50),
    ("100 snaps (~25 s)", 100),
    ("150 snaps (~37.5 s)", 150),
    ("200 snaps (~50 s)", 200),
)
REWIND_INTERVAL_CHOICES = (
    ("Every 15 frames", 15),
    ("Every 12 frames", 12),
    ("Every 8 frames", 8),
    ("Every 4 frames", 4),
    ("Every frame", 1),
)
# Defaults that reproduce today's behaviour exactly (the runtime's own defaults) so a player who
# never opens the section sees no change.
VIDEO_PERF_DEFAULTS = {
    ("video", "vsync"): "on",
    ("video", "low_latency_input"): True,
    ("video", "frame_interpolation_fps"): 0,
    ("video", "rewind_depth"): 50,
    ("video", "rewind_interval"): 15,
}
# Toggles the runtime reads from settings.toml [video]. low_latency_input re-samples the pad after
# the wall-clock pacer (just before present) so the next CPU frame reads fresh input; the engine
# default is on.
PERF_BOOLS = (
    ("low_latency_input", "Low-latency input"),
)

# Env-only presentation knobs: the runtime reads these from the environment and has NO settings.toml
# key for them (grep confirms), so the launcher stores them in its own config and exports them at
# launch. Each is (env name, plain-language label). PSX_REWIND=0 disables the rewind ring entirely
# (psx_rewind.c rewind_wanted); PSX_FRAME_BLEND and PSX_SMOOTH_60FPS default off; the FMV interval
# defaults to 4. Setting the default values explicitly is behaviour-identical to leaving them unset.
ENV_BOOLS = (
    ("PSX_SMOOTH_60FPS", "Smooth 60 FPS"),
    ("PSX_FRAME_BLEND", "Blend adjacent frames"),
    ("PSX_REWIND", "Rewind"),
)
ENV_BOOL_DEFAULTS = {
    "PSX_SMOOTH_60FPS": False,
    "PSX_FRAME_BLEND": False,
    # Rewind is off unless a player turns it on. Snapshots cost work every interval, the feature is
    # a convenience rather than part of the game, and it is the first thing to suspect when a build
    # feels slow. The launch path states the off value explicitly, see env_for_launch.
    "PSX_REWIND": False,
}
ENV_INT_CHOICES = (
    ("PSX_REWIND_FMV_INTERVAL", "Rewind FMV interval", ("4", "8", "12", "16")),
)
ENV_INT_DEFAULTS = {
    "PSX_REWIND_FMV_INTERVAL": "4",
}

# The launcher's own config (launcher.json) keeps the env-only knobs, keyed by build region, under
# this key. It is the launcher's file, not the runtime's, so nothing here reaches settings.toml.
VIDEO_PERF_ENV_KEY = "video_perf_env"


def load_env(region: str) -> dict[str, str]:
    """The env-only presentation knobs saved for one build region. Never raises."""
    try:
        from . import paths
        cfg = paths.load_config()
    except Exception:  # noqa: BLE001 - a broken config must not take the tab down
        return {}
    table = cfg.get(VIDEO_PERF_ENV_KEY) or {}
    values = table.get(region) or {}
    return {str(k): str(v) for k, v in dict(values).items()}


def save_env(region: str, values: dict[str, object]) -> None:
    """Persist the env-only knobs for one build region into the launcher's own config."""
    from . import paths
    cfg = paths.load_config()
    table = dict(cfg.get(VIDEO_PERF_ENV_KEY) or {})
    table[str(region)] = {str(k): str(v) for k, v in values.items()}
    cfg[VIDEO_PERF_ENV_KEY] = table
    paths.save_config(cfg)


def env_for_launch(region: str) -> dict[str, str]:
    """The environment to export for a launch of `region`, or {} when nothing is configured.

    builds.launch merges this onto the child's environment, so a control the player never touched
    adds nothing and changes nothing. The one exception is rewind: it is exported as OFF unless the
    player has turned it on, so the tick box and what the game actually runs can never disagree.
    """
    env = load_env(region)
    env.setdefault("PSX_REWIND", "0")
    return env
CONTROLLER_FIELDS = (
    ("p1_device", "Player 1 device"),
    ("p1_mode", "Player 1 mode"),
    ("p2_device", "Player 2 device"),
    ("p2_mode", "Player 2 mode"),
)


def available() -> bool:
    return tomlkit is not None


def load(path: pathlib.Path):
    """Parse the settings file, preserving comments and ordering.

    A missing file is an empty document rather than an error. The runtime creates settings.toml on a
    build's first clean run, and the Settings tab has to be able to write one before that has
    happened, because it shows the defaults the runtime will use until a file exists.
    """
    if tomlkit is None:
        raise RuntimeError("tomlkit is not installed (pip install tomlkit)")
    if not path.is_file():
        return tomlkit.document()
    text = path.read_text(encoding="utf-8-sig")
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

    The file is created when it does not exist yet, with the values handed in. A missing build
    FOLDER is refused with a plain reason instead: there is no executable there for the settings to
    belong to, so creating one would only make a build that is not really there.
    """
    if not path.parent.is_dir():
        raise RuntimeError(
            f"The {path.parent.name} build has no folder yet, so there is nothing to write. "
            f"Build that region's disc on the Play tab first: settings live in {path.parent} "
            f"beside the game's executable.")
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
