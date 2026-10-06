"""Filesystem layout for the DW3 launcher.

Everything is discovered relative to the launcher itself: the app root is derived from this file,
and the save editor the Memory Card tab embeds ships beside the launcher in ``editor/``. No path is
hard-coded to one machine.

The two regional game builds live in ``Builds/`` and are described by ``builds.py``. The launcher
no longer reads a third-party port install, so nothing here points outside the tree.
"""
from __future__ import annotations

import json
import os
import pathlib
import tempfile

APP_NAME = "DW3 Recompiled+"
APP_ID = "DMW3Launcher"

# The save editor is a published project of its own. It ships beside the launcher as ``editor/``
# and is imported live from there, so an editor fix reaches this launcher with no copying. A
# developer's own checkout is kept only as a last-resort fallback for someone working on the editor
# and the launcher together; it is never how a shipped build finds the editor.
_LEGACY_EDITOR_DIRS = (
    r"D:\AGENT\NEWEST DIGIMON WORLD 3 COMPLETE PROJECT\Digimon World 3 Save Editor [dev]",
    r"D:\AGENT\Digimon World 3 Save Editor [dev]",
)


def launcher_root() -> pathlib.Path:
    return pathlib.Path(__file__).resolve().parent.parent


def assets_dir() -> pathlib.Path:
    return launcher_root() / "assets"


def docs_dir() -> pathlib.Path:
    return launcher_root() / "docs"


def cards_dir() -> pathlib.Path:
    """Memory cards the player drops beside the launcher, alongside Builds/ and Discs/."""
    return launcher_root() / "cards"


def editor_dir() -> pathlib.Path:
    """The save editor shipped beside the launcher."""
    return launcher_root() / "editor"


def config_path() -> pathlib.Path:
    """Portable: the launcher keeps its own config beside itself."""
    return launcher_root() / "launcher.json"


def user_data_dir() -> pathlib.Path:
    """A writable per-user folder the launcher keeps its own data in.

    LOCALAPPDATA is not guaranteed: a stripped build/test environment (MSYS make, a service) can
    drop it, and `Path.home()` raises RuntimeError when neither HOME nor USERPROFILE is set. Neither
    is assumed, and `os.environ.get("LOCALAPPDATA", Path.home())` was worse than it looks, because
    that default is evaluated eagerly and crashed even when LOCALAPPDATA was present.
    """
    env = os.environ.get("LOCALAPPDATA")
    if env:
        base = pathlib.Path(env)
    else:
        try:
            base = pathlib.Path.home()
        except RuntimeError:
            base = pathlib.Path(tempfile.gettempdir())
    base = base / APP_ID
    base.mkdir(parents=True, exist_ok=True)
    return base


# ---------------------------------------------------------------- the embedded save editor

def _looks_like_editor_tree(path: pathlib.Path) -> bool:
    """An editor tree carries the importable package the Memory Card tab imports."""
    return (path / "dmw3editor" / "core" / "save.py").is_file()


def find_editor() -> pathlib.Path | None:
    """The save editor tree: the bundled ``editor/`` first, a dev checkout only as a fallback."""
    bundled = editor_dir()
    if _looks_like_editor_tree(bundled):
        return bundled
    for cand in _LEGACY_EDITOR_DIRS:
        p = pathlib.Path(cand)
        if _looks_like_editor_tree(p):
            return p
    return None


# ---------------------------------------------------------------- launcher config

_DEFAULT = {"version": 1, "editor_root": None}


def load_config() -> dict:
    p = config_path()
    if not p.is_file():
        return dict(_DEFAULT)
    try:
        cfg = json.loads(p.read_text(encoding="utf-8"))
    except Exception:
        cfg = dict(_DEFAULT)
    for key, val in _DEFAULT.items():
        cfg.setdefault(key, val)
    return cfg


def save_config(cfg: dict) -> None:
    config_path().write_text(json.dumps(cfg, indent=2), encoding="utf-8")


def resolve_editor(cfg: dict | None = None) -> pathlib.Path | None:
    """The editor tree to embed: a config override first, then the bundled ``editor/``."""
    cfg = cfg if cfg is not None else load_config()
    root = cfg.get("editor_root")
    if root:
        p = pathlib.Path(root)
        if _looks_like_editor_tree(p):
            return p
    return find_editor()
