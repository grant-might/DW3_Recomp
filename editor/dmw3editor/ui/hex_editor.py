"""Embedded hex editor for the DMW3 save editor (developer-locked).

This replaces the old read-only hex "viewer". It is a real byte editor over
the in-memory save payload: click a byte, type a new 2-digit hex value, and
the byte is written through :meth:`DMW3Save.set_byte`, which applies the same
region guards as every other edit (unverified regions raise SaveError and the
cell snaps back). Editing commits immediately to the staged save; both chunk
checksums are recomputed on save like every other edit.

The page is locked behind the Developer mode toggle in the sidebar: when
Developer mode is off the editor is read-only and shows a lock notice.
"""

from __future__ import annotations

import sys

from PySide6.QtCore import Qt
from PySide6.QtGui import QColor, QFont
from PySide6.QtWidgets import (
    QHBoxLayout,
    QLabel,
    QLineEdit,
    QMessageBox,
    QPushButton,
    QSpinBox,
    QTableWidget,
    QTableWidgetItem,
    QVBoxLayout,
    QWidget,
)

sys.path.insert(0, __import__("pathlib").Path(__file__).resolve().parents[2])

from dmw3editor.core import checksum as ck
from dmw3editor.core.save import (
    FORBIDDEN_REGIONS,
    PAYLOAD_SIZE,
    SaveError,
)

ROWS_PER_PAGE = 16
BYTES_PER_ROW = 16
BYTES_PER_PAGE = ROWS_PER_PAGE * BYTES_PER_ROW


def _in_forbidden(offset: int) -> bool:
    return any(start <= offset < end for start, end in FORBIDDEN_REGIONS)


class HexEditorPage(QWidget):
    """Paged hex editor over the current save payload."""

    def __init__(self) -> None:
        super().__init__()
        self._save = None
        self._page = 0
        self._dev_mode = False
        self._busy = False

        root = QVBoxLayout(self)
        root.setContentsMargins(20, 16, 20, 16)
        root.setSpacing(12)

        # status / toolbar -------------------------------------------------
        top = QHBoxLayout()
        top.setSpacing(8)
        self.lock_note = QLabel()
        self.lock_note.setProperty("role", "warning")
        self.lock_note.setWordWrap(True)
        top.addWidget(self.lock_note, 1)
        self.checksum_lbl = QLabel("")
        self.checksum_lbl.setProperty("role", "hint")
        top.addWidget(self.checksum_lbl)
        root.addLayout(top)

        nav = QHBoxLayout()
        nav.setSpacing(8)
        nav.addWidget(QLabel("Offset:"))
        self.goto_edit = QLineEdit()
        self.goto_edit.setPlaceholderText("0x…")
        self.goto_edit.setFixedWidth(110)
        self.goto_edit.returnPressed.connect(self._goto)
        nav.addWidget(self.goto_edit)
        go = QPushButton("Go")
        go.clicked.connect(self._goto)
        nav.addWidget(go)
        nav.addSpacing(10)
        prev = QPushButton("◀ Prev")
        prev.clicked.connect(lambda: self._set_page(self._page - 1))
        nav.addWidget(prev)
        self.page_lbl = QLabel("Page 0 / 0")
        self.page_lbl.setProperty("role", "hint")
        nav.addWidget(self.page_lbl)
        nxt = QPushButton("Next ▶")
        nxt.clicked.connect(lambda: self._set_page(self._page + 1))
        nav.addWidget(nxt)
        nav.addStretch(1)
        self.unlock_hint = QLabel("Developer mode is OFF, read-only.")
        self.unlock_hint.setProperty("role", "hint")
        nav.addWidget(self.unlock_hint)
        root.addLayout(nav)

        # grid ---------------------------------------------------------------
        self.table = QTableWidget(ROWS_PER_PAGE, 17)
        self.table.setHorizontalHeaderLabels(
            ["Offset"] + [f"{i:02X}" for i in range(BYTES_PER_ROW)]
        )
        self.table.verticalHeader().setVisible(False)
        self.table.setEditTriggers(QTableWidget.DoubleClicked | QTableWidget.EditKeyPressed)
        self.table.setFont(QFont("Cascadia Mono", 10))
        self.table.verticalHeader().setDefaultSectionSize(22)
        self.table.horizontalHeader().setDefaultSectionSize(42)
        self.table.setColumnWidth(0, 86)
        for col in range(1, 17):
            self.table.setColumnWidth(col, 42)
        self.table.itemChanged.connect(self._on_cell_changed)
        root.addWidget(self.table, 1)

        hint = QLabel(
            "Read-only regions (title frame, unverified regions) are greyed "
            "out. Editing a byte stages it in memory, Save the card to write "
            "it out and recompute both checksums. Typing a value also moves "
            "the cursor to the next byte."
        )
        hint.setWordWrap(True)
        hint.setProperty("role", "hint")
        root.addWidget(hint)

    # -- public API ---------------------------------------------------------
    def bind(self, save) -> None:
        """Bind to a loaded DMW3Save and show the first page."""
        self._save = save
        self._busy = True
        self.goto_edit.setText("0x0200")
        self._busy = False
        self._refresh_status()
        self._set_page(0)

    def set_dev_mode(self, enabled: bool) -> None:
        self._dev_mode = enabled
        self._refresh_lock()

    def _refresh_lock(self) -> None:
        on = self._dev_mode
        self.unlock_hint.setText(
            "Developer mode ON, editing enabled."
            if on
            else "Developer mode is OFF, read-only."
        )
        self.lock_note.setText(
            ""
            if on
            else "The hex editor is locked. Enable Developer mode in the "
            "sidebar footer to edit raw bytes."
        )
        self.lock_note.setVisible(not on)
        self.table.setEnabled(on)
        # go/nav stay usable for inspection even when locked
        if not on:
            # make cells read-only visually: clear edit triggers
            self.table.setEditTriggers(QTableWidget.NoEditTriggers)
        else:
            self.table.setEditTriggers(
                QTableWidget.DoubleClicked | QTableWidget.EditKeyPressed
            )

    # -- paging / status ----------------------------------------------------
    def _page_count(self) -> int:
        return max(1, (PAYLOAD_SIZE + BYTES_PER_PAGE - 1) // BYTES_PER_PAGE)

    def _set_page(self, page: int) -> None:
        if self._save is None:
            return
        pc = self._page_count()
        self._page = max(0, min(page, pc - 1))
        self._busy = True
        payload = self._save_bytes()
        base = self._page * BYTES_PER_PAGE
        self.table.setRowCount(ROWS_PER_PAGE)
        for row in range(ROWS_PER_PAGE):
            off = base + row * BYTES_PER_ROW
            off_item = QTableWidgetItem(f"{off:04X}")
            off_item.setForeground(QColor("#6b7280"))
            off_item.setFlags(Qt.ItemIsEnabled)
            self.table.setItem(row, 0, off_item)
            for col in range(BYTES_PER_ROW):
                poff = off + col
                cell = QTableWidgetItem(
                    f"{payload[poff]:02X}" if poff < len(payload) else "  "
                )
                cell.setTextAlignment(Qt.AlignCenter)
                if poff < 0x0200 or poff >= len(payload) or _in_forbidden(poff):
                    cell.setForeground(QColor("#4b5563"))
                    cell.setFlags(Qt.ItemIsEnabled)  # read-only
                    cell.setToolTip("Read-only region")
                else:
                    cell.setForeground(QColor("#eceef1"))
                    cell.setFlags(Qt.ItemIsEnabled | Qt.ItemIsEditable | Qt.ItemIsSelectable)
                self.table.setItem(row, 1 + col, cell)
        self.page_lbl.setText(
            f"Page {self._page + 1} / {pc}  ·  {base:04X}-"
            f"{min(base + BYTES_PER_PAGE, PAYLOAD_SIZE) - 1:04X}"
        )
        self._busy = False
        self._refresh_status()

    def _save_bytes(self) -> bytes:
        assert self._save is not None
        return self._save.to_bytes()

    def _refresh_status(self) -> None:
        if self._save is None:
            self.checksum_lbl.setText("")
            return
        data = self._save_bytes()
        h = ck.verify_header(data)
        c2 = ck.verify_chunk2(data)
        st = "valid" if h and c2 else "STALE"
        color = "#4ade80" if h and c2 else "#e6b450"
        self.checksum_lbl.setText(
            f"Checksums: {st}  (header {'✓' if h else '✗'}  "
            f"chunk2 {'✓' if c2 else '✗'})"
        )
        self.checksum_lbl.setStyleSheet(f"color: {color};")

    def _goto(self) -> None:
        if self._save is None:
            return
        text = self.goto_edit.text().strip()
        try:
            value = int(text, 0) if text.lower().startswith("0x") else int(text, 16)
        except ValueError:
            QMessageBox.warning(self, "Invalid offset", f"Cannot parse '{text}'.")
            return
        if value < 0 or value >= PAYLOAD_SIZE:
            QMessageBox.warning(
                self, "Offset out of range", f"Offset must be 0..0x{PAYLOAD_SIZE - 1:X}."
            )
            return
        self._set_page(value // BYTES_PER_PAGE)
        row = (value % BYTES_PER_PAGE) // BYTES_PER_ROW
        col = 1 + (value % BYTES_PER_ROW)
        if 0 <= row < self.table.rowCount():
            self.table.setCurrentCell(row, col)
            self.table.scrollToItem(self.table.item(row, col))

    # -- editing -------------------------------------------------------------
    def _on_cell_changed(self, item: QTableWidgetItem) -> None:
        if self._busy or self._save is None or not self._dev_mode:
            return
        if item.column() < 1 or item.column() > 16:
            return
        row, col = item.row(), item.column()
        off = self._page * BYTES_PER_PAGE + row * BYTES_PER_ROW + (col - 1)
        text = item.text().strip()
        try:
            value = int(text, 16)
        except ValueError:
            self._revert_cell(item, off)
            return
        if not 0 <= value <= 0xFF:
            self._revert_cell(item, off)
            return
        if not (0x0200 <= off < PAYLOAD_SIZE) or _in_forbidden(off):
            self._revert_cell(item, off)
            return
        try:
            self._save.set_byte(off, value)
        except SaveError:
            self._revert_cell(item, off)
            return
        item.setText(f"{value:02X}")
        self._refresh_status()
        win = self.window()
        if hasattr(win, "_mark_dirty"):
            win._mark_dirty()
        # advance to next byte
        ncol = col + 1
        if ncol <= 16:
            nxt = self.table.item(row, ncol)
            if nxt is not None and (nxt.flags() & Qt.ItemIsEditable):
                self.table.setCurrentCell(row, ncol)
        elif row + 1 < self.table.rowCount():
            self.table.setCurrentCell(row + 1, 1)

    def _revert_cell(self, item: QTableWidgetItem, off: int) -> None:
        self._busy = True
        try:
            payload = self._save_bytes()
            item.setText(f"{payload[off]:02X}" if off < len(payload) else "  ")
        finally:
            self._busy = False
