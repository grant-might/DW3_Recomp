"""The Mods tab.

A mod is a folder or file dropped into a build's own `mods/` folder. Enabling/disabling is a move
between `mods/` and `mods/.disabled/`, which keeps the built game tree itself pristine and makes a
mod's effect reversible without rebuilding. The rules the tab enforces are written out in
docs/MODS.md and shipped alongside.

Each regional build is its own install with its own `mods/` folder, so the tab works on one build at
a time; the region picker leads with USA, the same order the Play tab uses.
"""
from __future__ import annotations

import pathlib
import shutil

from PySide6.QtCore import Qt
from PySide6.QtWidgets import (QComboBox, QGroupBox, QHBoxLayout, QLabel, QListWidget,
                               QListWidgetItem, QMessageBox, QPushButton, QTextBrowser,
                               QVBoxLayout, QWidget)

from .. import builds, paths, runtime
from . import theme

RULES_SUMMARY = (
    "• A mod lives in one folder under the build's mods/ and must not write outside the build.\n"
    "• Mods may replace or add files under the build tree; they must never touch the memory "
    "cards (card1.mcd / card2.mcd), which are the player's data and belong to the Memory Card tab.\n"
    "• Ship the original file alongside a replacement as <name>.orig-stock so the mod can be "
    "reverted, the project already uses that convention.\n"
    "• A mod must not require a network connection at run time and must not modify the "
    "recompiled exe.\n"
    "• Saves made with a mod active may not load without it. Say so in the mod's README.\n"
)


class ModsTab(QWidget):
    def __init__(self, cfg: dict) -> None:
        super().__init__()
        self.cfg = cfg
        self.region: str = builds.default_region()

        lay = QVBoxLayout(self)
        lay.setContentsMargins(theme.PAGE_MARGIN, theme.CARD_GAP,
                               theme.PAGE_MARGIN, theme.CARD_GAP)
        lay.setSpacing(theme.CARD_GAP)

        self.head = QLabel()
        self.head.setWordWrap(True)
        self.head.setObjectName("hint")
        lay.addWidget(self.head)

        pick = QHBoxLayout()
        pick.setSpacing(theme.ROW_GAP)
        pick.addWidget(QLabel("Build"))
        self.combo_region = QComboBox()
        for b in builds.BUILDS:
            self.combo_region.addItem(b.label, b.region)
        self.combo_region.currentIndexChanged.connect(self._pick_region)
        pick.addWidget(self.combo_region)
        pick.addStretch(1)
        lay.addLayout(pick)

        box = QGroupBox("Installed mods")
        bl = QVBoxLayout(box)
        bl.setContentsMargins(theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD)
        bl.setSpacing(theme.ROW_GAP)
        self.list_mods = QListWidget()
        self.list_mods.setMinimumHeight(180)
        bl.addWidget(self.list_mods)
        row = QHBoxLayout()
        row.setSpacing(theme.ROW_GAP)
        self.btn_toggle = QPushButton("Enable / disable")
        self.btn_toggle.clicked.connect(self.toggle)
        self.btn_folder = QPushButton("Open mods folder")
        self.btn_folder.clicked.connect(self._open_mods)
        self.btn_refresh = QPushButton("Refresh")
        self.btn_refresh.clicked.connect(self.refresh)
        row.addWidget(self.btn_toggle)
        row.addWidget(self.btn_folder)
        row.addWidget(self.btn_refresh)
        row.addStretch(1)
        bl.addLayout(row)
        # the list of mods is the working area; the rules below are reference, so the list grows
        lay.addWidget(box, 1)

        gbox = QGroupBox("Rules for mods")
        gl = QVBoxLayout(gbox)
        gl.setContentsMargins(theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD)
        gl.setSpacing(theme.ROW_GAP)
        self.guide = QTextBrowser()
        self.guide.setOpenExternalLinks(True)
        self.guide.setMaximumHeight(150)
        gl.addWidget(self.guide)
        row2 = QHBoxLayout()
        row2.setSpacing(theme.ROW_GAP)
        self.btn_guide = QPushButton("Open full guide (docs/MODS.md)")
        self.btn_guide.clicked.connect(self._open_guide)
        row2.addWidget(self.btn_guide)
        self.btn_mkmod = QPushButton("Create empty mod skeleton…")
        self.btn_mkmod.clicked.connect(self._skeleton)
        row2.addWidget(self.btn_mkmod)
        row2.addStretch(1)
        gl.addLayout(row2)
        lay.addWidget(gbox)

        self.guide.setPlainText(RULES_SUMMARY)
        index = self.combo_region.findData(self.region)
        if index >= 0:
            self.combo_region.blockSignals(True)
            self.combo_region.setCurrentIndex(index)
            self.combo_region.blockSignals(False)
        self.refresh()

    # ------------------------------------------------------------------ helpers
    def _pick_region(self) -> None:
        self.region = self.combo_region.currentData() or builds.default_region()
        self.refresh()

    def _dirs(self) -> tuple[pathlib.Path, pathlib.Path]:
        on = builds.mods_dir(self.region)
        off = on / ".disabled"
        return on, off

    def refresh(self) -> None:
        self.list_mods.clear()
        if not builds.build_status(self.region)[0]:
            label = builds.spec(self.region).label
            self.head.setText(f"The {label} build is not on disk yet, build it on the Play tab, "
                              f"then mods can be dropped into its mods/ folder.")
            self.btn_toggle.setEnabled(False)
            self.btn_folder.setEnabled(False)
            self.btn_mkmod.setEnabled(False)
            return
        self.btn_toggle.setEnabled(True)
        self.btn_folder.setEnabled(True)
        self.btn_mkmod.setEnabled(True)
        on, off = self._dirs()
        on.mkdir(parents=True, exist_ok=True)
        self.head.setText(f"Mods live in {on}")
        for base, state in ((on, "enabled"), (off, "disabled")):
            if not base.is_dir():
                continue
            for p in sorted(base.iterdir()):
                if p.name.startswith("."):
                    continue
                it = QListWidgetItem(f"[{state}]  {p.name}"
                                     + ("  (folder)" if p.is_dir() else ""))
                it.setData(Qt.ItemDataRole.UserRole, (state, str(p)))
                self.list_mods.addItem(it)
        if self.list_mods.count() == 0:
            self.list_mods.addItem("No mods installed yet.")

    def toggle(self) -> None:
        it = self.list_mods.currentItem()
        if not it:
            return
        data = it.data(Qt.ItemDataRole.UserRole)
        if not data:
            return
        state, path = data
        src = pathlib.Path(path)
        on, off = self._dirs()
        dst_dir = off if state == "enabled" else on
        dst_dir.mkdir(parents=True, exist_ok=True)
        try:
            shutil.move(str(src), str(dst_dir / src.name))
        except Exception as exc:  # noqa: BLE001
            QMessageBox.critical(self, "Could not move mod", str(exc))
        self.refresh()

    def _open_mods(self) -> None:
        if not builds.build_status(self.region)[0]:
            return
        on, _ = self._dirs()
        on.mkdir(parents=True, exist_ok=True)
        runtime.open_path(on)

    def _open_guide(self) -> None:
        runtime.open_path(paths.docs_dir() / "MODS.md")

    def _skeleton(self) -> None:
        if not builds.build_status(self.region)[0]:
            return
        on, _ = self._dirs()
        base = on / "my-mod"
        n = 1
        while base.exists():
            n += 1
            base = on / f"my-mod-{n}"
        base.mkdir(parents=True, exist_ok=True)
        (base / "README.md").write_text(
            f"# {base.name}\n\nWhat this mod changes, and how to uninstall it.\n\n"
            f"## Files it replaces\n\n- `path/inside/build/modified.file` "
            f"(original kept as `modified.file.orig-stock`)\n\n"
            f"## Does it affect saves?\n\nNo / Yes, explain.\n", encoding="utf-8")
        self.refresh()
        runtime.open_path(base)
