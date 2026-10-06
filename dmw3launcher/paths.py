"""Filesystem layout for the DW3 launcher.

Everything is discovered, nothing is hard-coded to one machine: the runtime (the recompiled
game install) can live anywhere and is recorded in the launcher config once located.
"""
from __future__ import annotations

import json
import os
import pathlib

APP_NAME = "DW3 Recompiled+"
APP_ID = "DMW3Launcher"

# Where the recompiled install is likely to be, newest-known first.
_RUNTIME_CANDIDATES = (
    r"D:\AGENT\Digimon World 3 Recomp\DigimonWorld2003",
    r"D:\AGENT\NEWEST DIGIMON WORLD 3 COMPLETE PROJECT\DigimonWorld2003",
    r"C:\Games\DigimonWorld2003",
)

# The save editor dev tree that we embed as the Memory Card tab.
_EDITOR_CANDIDATES = (
    r"D:\AGENT\NEWEST DIGIMON WORLD 3 COMPLETE PROJECT\Digimon World 3 Save Editor [dev]",
    r"D:\AGENT\Digimon World 3 Save Editor [dev]",
)


def launcher_root() -> pathlib.Path:
    return pathlib.Path(__file__).resolve().parent.parent


def assets_dir() -> pathlib.Path:
    return launcher_root() / "assets"


def docs_dir() -> pathlib.Path:
    return launcher_root() / "docs"


def config_path() -> pathlib.Path:
    """Portable: the launcher keeps its own config beside itself."""
    return launcher_root() / "launcher.json"


def user_data_dir() -> pathlib.Path:
    base = pathlib.Path(os.environ.get("LOCALAPPDATA", pathlib.Path.home())) / APP_ID
    base.mkdir(parents=True, exist_ok=True)
    return base


# ---------------------------------------------------------------- runtime install

def looks_like_runtime(path: pathlib.Path) -> bool:
    """A runtime install has the exe, the config and the extracted disc tree."""
    return (
        (path / "Binaries" / "DigimonWorld2003.exe").is_file()
        and (path / "Engine" / "Config" / "game.toml").is_file()
    )


def find_runtime() -> pathlib.Path | None:
    for cand in _RUNTIME_CANDIDATES:
        p = pathlib.Path(cand)
        if looks_like_runtime(p):
            return p
    return None


def bin_dir(root: pathlib.Path) -> pathlib.Path:
    return root / "Binaries"


def exe_path(root: pathlib.Path) -> pathlib.Path:
    return bin_dir(root) / "DigimonWorld2003.exe"


def settings_toml(root: pathlib.Path) -> pathlib.Path:
    return bin_dir(root) / "settings.toml"


def game_toml(root: pathlib.Path) -> pathlib.Path:
    return root / "Engine" / "Config" / "game.toml"


def idioma_ini(root: pathlib.Path) -> pathlib.Path:
    return root / "Engine" / "Config" / "idioma.ini"


def savedata_dir(root: pathlib.Path) -> pathlib.Path:
    """Where the runtime keeps its saves (game.toml sets memcard_dir to this)."""
    return root / "DMW3Game" / "SAVEDATA"


def discs_dir(root: pathlib.Path) -> pathlib.Path:
    return root / "discs"


def mods_dir(root: pathlib.Path) -> pathlib.Path:
    """Mod payloads: files/modules the launcher can enable or disable."""
    return root / "mods"


def disc_tool(root: pathlib.Path) -> pathlib.Path:
    """DiscTool ships at the top of the recomp project, one level above the install."""
    for cand in (root.parent / "DiscTool.exe", root / "DiscTool.exe",
                 launcher_root() / "tools" / "DiscTool.exe"):
        if cand.is_file():
            return cand
    return root.parent / "DiscTool.exe"


def find_editor() -> pathlib.Path | None:
    """The save-editor dev tree (importable package lives under it)."""
    for cand in _EDITOR_CANDIDATES:
        p = pathlib.Path(cand)
        if (p / "dmw3editor" / "core" / "save.py").is_file():
            return p
    return None


# ---------------------------------------------------------------- launcher config

_DEFAULT = {"version": 1, "runtime_root": None, "editor_root": None, "profiles": {}, "active": None}


def load_config() -> dict:
    p = config_path()
    if not p.is_file():
        cfg = dict(_DEFAULT)
        cfg["profiles"] = {}
        return cfg
    try:
        cfg = json.loads(p.read_text(encoding="utf-8"))
    except Exception:
        cfg = dict(_DEFAULT)
    for key, val in _DEFAULT.items():
        cfg.setdefault(key, val)
    return cfg


def save_config(cfg: dict) -> None:
    config_path().write_text(json.dumps(cfg, indent=2), encoding="utf-8")


def resolve_runtime(cfg: dict | None = None) -> pathlib.Path | None:
    cfg = cfg if cfg is not None else load_config()
    root = cfg.get("runtime_root")
    if root:
        p = pathlib.Path(root)
        if looks_like_runtime(p):
            return p
    return find_runtime()


def resolve_editor(cfg: dict | None = None) -> pathlib.Path | None:
    cfg = cfg if cfg is not None else load_config()
    root = cfg.get("editor_root")
    if root:
        p = pathlib.Path(root)
        if (p / "dmw3editor").is_dir():
            return p
    return find_editor()
