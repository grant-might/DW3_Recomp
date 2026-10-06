"""Launching the recompiled runtime, and reading back what it is doing."""
from __future__ import annotations

import os
import pathlib
import subprocess
import sys

from . import paths


class RuntimeError_(RuntimeError):
    pass


# The game refuses to run unless its own launcher started it. DigimonWorld2003.exe looks for this
# environment variable and otherwise shows "Please start the game from the Launcher. Run
# Lanzador.exe instead of this file." Binaries/Launcher.exe (the Spanish "Lanzador") is what sets
# it, along with its own UI state (PSX_MODO, PSX_LOGROS, PSX_TROFEOS, PSX_TROFEOS_ENTRADA,
# PSX_DISC_MAP, DMW3_LAUNCH_LOG). This launcher does the Lanzador's job, so it has to present the
# same marker; the other variables are UI state that this launcher owns via settings.toml and
# game.toml rather than by exporting them.
LAUNCHER_MARKER = "DMW3_DESDE_LANZADOR"


def launch(root: pathlib.Path, extra_args: list[str] | None = None) -> subprocess.Popen:
    """Start the game.

    The exe locates its own install root by walking up from itself to find
    Engine/Config/game.toml, so the working directory is not load-bearing — but Binaries/ is
    where it expects its own state, so that is what we use.
    """
    exe = paths.exe_path(root)
    if not exe.is_file():
        raise RuntimeError_(f"Runtime executable not found: {exe}")
    cwd = paths.bin_dir(root)
    creation = 0
    if sys.platform == "win32":
        creation = getattr(subprocess, "CREATE_NEW_PROCESS_GROUP", 0)
    env = dict(os.environ)
    env[LAUNCHER_MARKER] = "1"
    return subprocess.Popen([str(exe), *(extra_args or [])], cwd=str(cwd), env=env,
                            creationflags=creation, close_fds=True)


def running_processes() -> list[str]:
    """Names of live game processes (best effort, no psutil dependency)."""
    if sys.platform != "win32":
        try:
            out = subprocess.run(["pgrep", "-f", "DigimonWorld2003"], capture_output=True,
                                 text=True).stdout
            return [x for x in out.split() if x.strip()]
        except Exception:
            return []
    try:
        out = subprocess.run(
            ["tasklist", "/FI", "IMAGENAME eq DigimonWorld2003.exe", "/FO", "CSV", "/NH"],
            capture_output=True, text=True, errors="replace").stdout
    except Exception:
        return []
    return [line.split(",")[0].strip('"') for line in out.splitlines()
            if "DigimonWorld2003" in line]


def input_files(root: pathlib.Path) -> dict[str, pathlib.Path]:
    """The files that hold controller/keyboard bindings — the launcher must never rewrite
    these, only preserve and reveal them."""
    b = paths.bin_dir(root)
    return {"input.ini": b / "input.ini", "keybinds.ini": b / "keybinds.ini"}


def controller_summary(root: pathlib.Path) -> dict[str, str]:
    """Current controller settings, shown read-only. Never raises."""
    try:
        from . import settings
        p = paths.settings_toml(root)
        if not p.is_file():
            return {}
        snap = settings.snapshot(p)
        return {k: str(v) for k, v in snap.get("controller", {}).items()}
    except Exception:
        return {}


def last_run_report(root: pathlib.Path) -> dict:
    """The runtime writes a run report when it exits; useful for a 'did it crash?' hint."""
    from . import settings  # noqa: F401  (kept for symmetry / future use)
    import json
    p = paths.bin_dir(root) / "psx_last_run_report.json"
    if not p.is_file():
        return {}
    try:
        d = json.loads(p.read_text(encoding="utf-8", errors="replace"))
    except Exception:
        return {}
    keep = ("reason", "exit_origin", "build", "timestamp", "frame", "last_func_addr")
    return {k: d.get(k) for k in keep if k in d}


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
