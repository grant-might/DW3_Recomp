"""Headless smoke test: build the whole launcher UI without opening a window.

Run with a Python that has PySide6 (the save editor's venv will do).
"""
import pathlib
import sys
import traceback

ROOT = pathlib.Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))

from PySide6.QtWidgets import QApplication  # noqa: E402

from dmw3launcher import builds, paths, saves  # noqa: E402
from dmw3launcher.ui import theme  # noqa: E402
from dmw3launcher.ui.main_window import MainWindow  # noqa: E402

fails = []

app = QApplication(sys.argv)
app.setStyleSheet(theme.stylesheet())
print("stylesheet applied")

cfg = paths.load_config()
print("config:", cfg)

print("builds:", ", ".join(f"{s.label}={'ready' if builds.build_status(s.region)[0] else 'missing'}"
                          for s in builds.BUILDS))
print("editor :", paths.resolve_editor(cfg))

try:
    win = MainWindow(cfg)
    print(f"window built; tabs = {win.tabs.count()}")
    for i in range(win.tabs.count()):
        print(f"  tab {i}: {win.tabs.tabText(i)}")
except Exception:
    fails.append("MainWindow")
    traceback.print_exc()

# the individual tabs, so a failure names itself
from dmw3launcher.ui.play_tab import PlayTab  # noqa: E402
from dmw3launcher.ui.settings_tab import SettingsTab  # noqa: E402
from dmw3launcher.ui.mods_tab import ModsTab  # noqa: E402
from dmw3launcher.ui.memcard_tab import MemcardTab  # noqa: E402

for name, cls in (("PlayTab", PlayTab), ("SettingsTab", SettingsTab),
                  ("ModsTab", ModsTab), ("MemcardTab", MemcardTab)):
    try:
        w = cls(cfg)
        print(f"{name}: ok")
    except Exception:
        fails.append(name)
        print(f"{name}: FAILED")
        traceback.print_exc()

found = saves.discover()
print(f"memory cards discovered: {len(found)}")
for f in found[:8]:
    print("   ", f.kind, f.path.name, f.size, "bytes")

print("RESULT:", "FAILURES: " + ", ".join(fails) if fails else "all tabs built")
