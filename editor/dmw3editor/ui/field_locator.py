"""Field Locator tab, the honest path to discovering UNVERIFIED save fields.

Some fields (item inventory, card collection, per-Digimon stat/XP blocks) were
NOT found by static analysis of two unrelated player saves: with two different
playthroughs there are thousands of statistically indistinguishable candidates.

The only reliable method is a CONTROLLED DIFF:

  1. Play the game normally. Save to memory card A.
  2. Do exactly ONE action (e.g. "use 1 Potion", "pick up 1 card").
  3. Save to memory card B.
  4. Load both here. The bytes that differ ARE the field for that action.

This tab never guesses and never writes, it only reports. Confirm the field in
game, then it can be added to the verified model.
"""

from __future__ import annotations

from PySide6.QtCore import Qt
from PySide6.QtGui import QFont
from PySide6.QtWidgets import (
    QFileDialog,
    QGroupBox,
    QHBoxLayout,
    QLabel,
    QPlainTextEdit,
    QPushButton,
    QVBoxLayout,
    QWidget,
)

from dmw3editor.core import diff as diffmod


class FieldLocator(QWidget):
    def __init__(self, parent: QWidget | None = None) -> None:
        super().__init__(parent)
        root = QVBoxLayout(self)
        root.setContentsMargins(20, 16, 20, 16)
        root.setSpacing(14)

        intro = QLabel(
            "Locate unknown fields by diffing two saves that differ by ONE "
            "in-game action. Load the unchanged save first, then the changed "
            "one. Changed bytes are reported below, read-only, never written."
        )
        intro.setWordWrap(True)
        intro.setObjectName("muted")
        root.addWidget(intro)

        pick = QGroupBox("Two saves")
        pl = QHBoxLayout(pick)
        self.before_btn = QPushButton("1. Unchanged save…")
        self.after_btn = QPushButton("2. Changed save…")
        self.before_lbl = QLabel("-")
        self.after_lbl = QLabel("-")
        self.before_lbl.setObjectName("muted")
        self.after_lbl.setObjectName("muted")
        self.diff_btn = QPushButton("Compute diff")
        self.diff_btn.setProperty("role", "primary")
        self.diff_btn.setEnabled(False)
        pl.addWidget(self.before_btn)
        pl.addWidget(self.before_lbl, 1)
        pl.addWidget(self.after_btn)
        pl.addWidget(self.after_lbl, 1)
        pl.addWidget(self.diff_btn)
        root.addWidget(pick)

        self.out = QPlainTextEdit()
        self.out.setReadOnly(True)
        self.out.setLineWrapMode(QPlainTextEdit.LineWrapMode.NoWrap)
        self.out.setFont(QFont("Consolas", 10))
        root.addWidget(self.out, 1)

        self.before_btn.clicked.connect(lambda: self._pick("before"))
        self.after_btn.clicked.connect(lambda: self._pick("after"))
        self.diff_btn.clicked.connect(self._run)

        self._before: str | None = None
        self._after: str | None = None

    def _pick(self, which: str) -> None:
        path, _ = QFileDialog.getOpenFileName(
            self, "Select save", "", "Saves (*.gme *.vgs *.mcr *.bin *.mcd);;All (*)"
        )
        if not path:
            return
        if which == "before":
            self._before = path
            self.before_lbl.setText(path.split("/")[-1].split("\\")[-1])
        else:
            self._after = path
            self.after_lbl.setText(path.split("/")[-1].split("\\")[-1])
        self.diff_btn.setEnabled(bool(self._before and self._after))

    def _run(self) -> None:
        if not self._before or not self._after:
            return
        try:
            report = diffmod.format_report(
                diffmod.diff_files(self._before, self._after)
            )
        except Exception as exc:  # noqa: BLE001
            self.out.setPlainText(f"error: {exc}")
            return
        self.out.setPlainText(report)
