"""Path resolution that works in BOTH the dev tree and a PyInstaller build.

PyInstaller (--onedir) places bundled data under sys._MEIPASS and keeps the
package at <bundle>/dmw3editor/. In the dev tree everything lives next to this
file. Call sites (assets, save data tables, logo, tick icons) use the
resolvers below so a frozen build finds the same files.
"""
from __future__ import annotations

import pathlib
import sys

_PKG = "dmw3editor"


def _bundle_root() -> pathlib.Path:
    """Top of the app bundle: _MEIPASS when the package is bundled under it, project root else.

    The frozen branch is correct for this project's OWN PyInstaller build, which ships
    ``dmw3editor/{assets,data}`` under ``sys._MEIPASS``. It is wrong when the package is imported
    live into a frozen HOST: the DW3 launcher embeds this editor as its Memory Card tab and puts
    this tree on ``sys.path``, so ``sys.frozen`` is the LAUNCHER's and ``_MEIPASS`` has no
    ``dmw3editor/`` at all. Every asset lookup then resolved to a missing path, its pixmaps came
    back null, and ``assets.scaled`` logged "Pixmap is a null pixmap" once per roster tile.
    Detect that case and resolve against the source tree this file actually lives in.
    """
    if getattr(sys, "frozen", False):  # PyInstaller sets sys.frozen
        base = pathlib.Path(sys._MEIPASS)  # noqa: SLF001
        if (base / _PKG).is_dir():
            return base
    return pathlib.Path(__file__).resolve().parent.parent


def pkg_dir() -> pathlib.Path:
    """The dmw3editor package directory (data assets live under it)."""
    return _bundle_root() / _PKG


def repo_root() -> pathlib.Path:
    """Bundle root / repo root (logo.png lives here in both layouts)."""
    return _bundle_root()


def writable_dir(rel: str) -> pathlib.Path:
    """A directory the app may WRITE at runtime (generated tick icons, caches).

    In the dev tree this is inside the package (so screenshots/QA see the same
    files); in a frozen build the bundle may be read-only (Program Files), so
    use the per-user local app data dir instead.
    """
    if getattr(sys, "frozen", False):
        base = pathlib.Path.home() / "AppData" / "Local" / "DMW3SaveEditor"
    else:
        base = pkg_dir()
    p = base / rel
    p.mkdir(parents=True, exist_ok=True)
    return p
