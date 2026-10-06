"""DW3 Recompiled+, the launcher for the native recompilation.

    python main.py
"""
from __future__ import annotations

import pathlib
import sys

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

from PySide6.QtWidgets import QApplication  # noqa: E402

from dmw3launcher import paths  # noqa: E402
from dmw3launcher.ui import theme  # noqa: E402
from dmw3launcher.ui.main_window import MainWindow, VERSION  # noqa: E402

# Windows groups a taskbar button by this id. Python's own process id is the default, so without a
# distinct one the taskbar button shows python.exe's icon no matter what the window's icon is set
# to. Kept stable: changing it orphans the user's pinned shortcut.
APP_USER_MODEL_ID = "DW3RecompiledPlus.Launcher"


def set_windows_app_id(app_id: str = APP_USER_MODEL_ID) -> bool:
    """Claim this app's own taskbar identity. Returns whether Windows accepted it."""
    if sys.platform != "win32":
        return False
    try:
        import ctypes  # noqa: PLC0415
        return ctypes.windll.shell32.SetCurrentProcessExplicitAppUserModelID(app_id) == 0
    except Exception:  # noqa: BLE001
        return False


def main() -> int:
    set_windows_app_id()          # before the first window exists, or the taskbar has already grouped
    app = QApplication(sys.argv)
    app.setApplicationName(paths.APP_NAME)
    app.setApplicationDisplayName(paths.APP_NAME)
    app.setWindowIcon(theme.app_icon())
    # The window carries the same icon, so the title bar, the taskbar and the alt-tab list agree.

    cfg = paths.load_config()
    win = MainWindow(cfg)
    win.show()
    return app.exec()


if __name__ == "__main__":
    raise SystemExit(main())
