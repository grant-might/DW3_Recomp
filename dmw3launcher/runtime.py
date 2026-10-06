"""Small OS-facing helpers shared by the launcher's tabs.

This module used to carry the contract for a third-party Spanish port (`DigimonWorld2003.exe` and
its `Lanzador.exe` launch marker). The builder replaced that port: the launcher now makes and runs
its own regional builds (see `builds.py`), so nothing here speaks to a foreign binary any more.
What is left is the one thing the tabs genuinely need: revealing a file or folder in the OS file
manager.
"""
from __future__ import annotations

import os
import pathlib
import subprocess
import sys


def open_path(path: pathlib.Path) -> None:
    """Reveal a file or folder in the OS file manager."""
    if not path.exists():
        return
    if sys.platform == "win32":
        if path.is_file():
            subprocess.Popen(["explorer", "/select,", str(path)])
        else:
            os.startfile(str(path))  # noqa: S606
    elif sys.platform == "darwin":
        subprocess.Popen(["open", str(path)])
    else:
        subprocess.Popen(["xdg-open", str(path)])


def controller_summary(settings_path: pathlib.Path) -> dict[str, str]:
    """A build's own `[controller]` settings, shown read-only. Never raises.

    Controller bindings live in the build's `input.ini` / `keybinds.ini`, which the launcher never
    rewrites; this is only the section the runtime also keeps in its `settings.toml`.
    """
    try:
        from . import settings
        if not settings_path.is_file():
            return {}
        snap = settings.snapshot(settings_path)
        return {k: str(v) for k, v in snap.get("controller", {}).items()}
    except Exception:  # noqa: BLE001
        return {}
