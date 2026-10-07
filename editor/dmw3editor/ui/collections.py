"""Collections tabs — items, key items, and the card collection.

Items: the save stores a byte-per-item quantity array at payload 0x03A7.
CONFIRMED (2026-09-02): all 352 slots are named from the decomp ItemId table
(save slot = decomp enum idx - 35). Subclass boundaries were confirmed by live
block tests: items 0-48, weapons 49-171, armor 172-248, accessories 249-316,
card packs 317-351.

Key Items: 48 owned-flags in the game's Important-screen order. Block A
(39 flags) at payload 0x0380..0x03A6, Monmon DDNA at main-array slot 317,
Block B (8 flags) at payload 0x0507..0x050E. CONFIRMED byte-exact against
the player's 35-item in-game read (2026-09-02).

Cards: 314 byte-per-card counts (0-9) at payload 0x06A3. CONFIRMED by two
live in-game anchors (index 0 = "Sacred Spear", index 50 = "White Remove").

Every edit stages in memory and is committed only when the user saves the
card; the model's to_bytes() recomputes both chunk checksums.
"""
from __future__ import annotations

import sys

from PySide6.QtCore import QSize, Qt
from PySide6.QtGui import QColor, QIcon, QPixmap
from PySide6.QtWidgets import (
    QAbstractItemView,
    QButtonGroup,
    QComboBox,
    QGridLayout,
    QGroupBox,
    QHBoxLayout,
    QHeaderView,
    QLabel,
    QLineEdit,
    QPushButton,
    QSpinBox,
    QTableWidget,
    QTableWidgetItem,
    QToolButton,
    QVBoxLayout,
    QWidget,
)

sys.path.insert(0, __import__("pathlib").Path(__file__).resolve().parents[2])

from dmw3editor.core.save import (
    CARD_BASE,
    CARD_COUNT,
    CARD_MAX,
    D_DV_COUNT,
    D_DV_HEADER_FORMS,
    D_DV_MAX,
    DIGI_ROSTER_NAMES,
    DIGI_STAT_COUNT,
    DIGI_STAT_NAMES,
    DIGI_MAX_EXP,
    DIGI_MAX_HP,
    DIGI_MAX_LEVEL,
    DIGI_MAX_MP,
    DIGI_MAX_STAT,
    DV_FORM_MARKER_NAMES,
    DV_FORM_MARKERS,
    ITEM_MAX,
    PACK_COUNT,
    TABLES,
    SaveError,
)


def _item_name_for(index: int) -> tuple[str, bool]:
    """Return (display_name, confirmed).

    All 352 slots are now named from the authoritative decomp ItemId table
    (save slot = decomp enum idx - 35). Indices 0-48 are additionally confirmed
    against the player's in-game item screen; the subclass boundaries for
    weapons/armor/accessories/important were confirmed by live block tests.
    """
    name = TABLES.item_name(index)
    confirmed = True
    return name, confirmed


class ItemCategoryPage(QWidget):
    """Quantity editor for one item subclass (items / weapons / armor / ...).

    Each category gets its own page (sidebar nav), showing ONLY its slot
    range from the shared 352-slot byte array at payload 0x03A7. A live
    name filter narrows rows without touching the data model.
    """

    # filled by subclasses
    TITLE = "Items"
    START = 0
    END = 49

    def __init__(self) -> None:
        super().__init__()
        self._save = None
        self._busy = False

        root = QVBoxLayout(self)
        root.setContentsMargins(20, 16, 20, 16)
        root.setSpacing(12)

        # filter row
        filter_row = QHBoxLayout()
        filter_row.setSpacing(8)
        filter_lbl = QLabel("Filter:")
        filter_lbl.setProperty("role", "hint")
        filter_row.addWidget(filter_lbl)
        self.filter_edit = QLineEdit()
        self.filter_edit.setPlaceholderText("Search by name…")
        self.filter_edit.setClearButtonEnabled(True)
        self.filter_edit.textChanged.connect(self._apply_filter)
        filter_row.addWidget(self.filter_edit, 1)
        self.count_lbl = QLabel("")
        self.count_lbl.setProperty("role", "hint")
        filter_row.addWidget(self.count_lbl)
        root.addLayout(filter_row)

        self.table = QTableWidget(0, 3)
        self.table.setHorizontalHeaderLabels(["Art", "Item", "Qty"])
        self.table.horizontalHeader().setSectionResizeMode(1, QHeaderView.Stretch)
        self.table.setColumnWidth(0, 56)
        self.table.setColumnWidth(2, 80)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table.setAlternatingRowColors(True)
        self.table.verticalHeader().setVisible(False)
        self.table.verticalHeader().setDefaultSectionSize(64)
        self.table.itemSelectionChanged.connect(self._on_selection)
        root.addWidget(self.table, 1)

        row = QHBoxLayout()
        row.setSpacing(8)
        self.sel_label = QLabel("No row selected")
        self.sel_label.setProperty("role", "hint")
        row.addWidget(self.sel_label)
        row.addStretch(1)
        row.addWidget(QLabel("Qty:"))
        self.qty_spin = QSpinBox()
        self.qty_spin.setRange(0, ITEM_MAX)
        self.qty_spin.setValue(ITEM_MAX)
        self.qty_spin.valueChanged.connect(self._on_qty_edit)
        row.addWidget(self.qty_spin)
        self.apply_btn = QPushButton("Apply to save")
        self.apply_btn.setProperty("role", "primary")
        self.apply_btn.clicked.connect(self._apply)
        row.addWidget(self.apply_btn)
        max_btn = QPushButton("Max this row")
        max_btn.clicked.connect(self._max_row)
        row.addWidget(max_btn)
        clear_btn = QPushButton("Zero this row")
        clear_btn.clicked.connect(self._zero_row)
        row.addWidget(clear_btn)
        root.addLayout(row)

    # -- public -----------------------------------------------------------
    def bind(self, save) -> None:
        self._save = save
        self._busy = True
        self._fill_table()
        self._busy = False
        self._apply_filter()
        self._select_row(0)

    # -- internal ---------------------------------------------------------
    def _fill_table(self) -> None:
        if self._save is None:
            return
        from dmw3editor.ui import assets

        n = self.END - self.START
        self.table.setRowCount(n)
        for row in range(n):
            slot = self.START + row
            name, _confirmed = _item_name_for(slot)
            art = QTableWidgetItem()
            pix = assets.item_icon_for_slot(slot)
            if pix is not None:
                art.setData(Qt.DecorationRole, assets.scaled(pix, 48))
                art.setToolTip(name)
            art.setTextAlignment(Qt.AlignCenter)
            self.table.setItem(row, 0, art)
            self.table.setItem(row, 1, QTableWidgetItem(name))
            qty = self._save.item_qty(slot)
            self.table.setItem(row, 2, QTableWidgetItem(str(qty)))
        self.table.resizeRowsToContents()

    def _apply_filter(self, _text: str | None = None) -> None:
        if self._save is None:
            return
        needle = self.filter_edit.text().strip().lower()
        shown = 0
        for row in range(self.table.rowCount()):
            name = self.table.item(row, 1).text().lower()
            hide = bool(needle) and needle not in name
            self.table.setRowHidden(row, hide)
            if not hide:
                shown += 1
        self.count_lbl.setText(f"{shown} of {self.table.rowCount()}")

    def _selected_slot(self) -> int | None:
        """Return the save slot behind the currently selected row, or None."""
        if self._save is None:
            return None
        rows = self.table.selectionModel().selectedRows()
        r = rows[0].row() if rows else self.table.currentRow()
        if r < 0 or r >= self.table.rowCount():
            return None
        if self.table.isRowHidden(r):
            return None
        return self.START + r

    def _select_row(self, row: int) -> None:
        if not 0 <= row < self.table.rowCount():
            self.apply_btn.setEnabled(False)
            self.sel_label.setText("No row selected")
            return
        self.table.selectRow(row)
        self.table.setCurrentCell(row, 1)
        self._sync_selection()

    def _on_selection(self, *_args) -> None:
        if self._busy:
            return
        self._sync_selection()

    def _sync_selection(self) -> None:
        slot = self._selected_slot()
        if slot is None:
            self.apply_btn.setEnabled(False)
            self.sel_label.setText("No row selected")
            return
        name, _confirmed = _item_name_for(slot)
        self.sel_label.setText(name)
        self.apply_btn.setEnabled(True)
        self._busy = True
        self.qty_spin.setValue(self._save.item_qty(slot))
        self._busy = False

    def _on_qty_edit(self, _value: int) -> None:
        # preview only; actual write on Apply
        if self._busy or self._save is None:
            return
        slot = self._selected_slot()
        if slot is None:
            return
        row = slot - self.START
        self.table.item(row, 2).setText(str(self.qty_spin.value()))

    def _apply(self) -> None:
        if self._save is None:
            return
        slot = self._selected_slot()
        if slot is None:
            return
        qty = self.qty_spin.value()
        try:
            self._save.set_item_qty(slot, qty)
        except SaveError as exc:
            self._sync_selection()
            return
        row = slot - self.START
        self.table.item(row, 2).setText(str(qty))
        # surface to parent dirty flag via a lightweight signal
        win = self.window()
        if hasattr(win, "_mark_dirty"):
            win._mark_dirty()

    def _max_row(self) -> None:
        if self._save is None:
            return
        slot = self._selected_slot()
        if slot is None:
            return
        self._save.set_item_qty(slot, ITEM_MAX)
        row = slot - self.START
        self.table.item(row, 2).setText(str(ITEM_MAX))
        self._busy = True
        self.qty_spin.setValue(ITEM_MAX)
        self._busy = False
        win = self.window()
        if hasattr(win, "_mark_dirty"):
            win._mark_dirty()

    def _zero_row(self) -> None:
        if self._save is None:
            return
        slot = self._selected_slot()
        if slot is None:
            return
        self._save.set_item_qty(slot, 0)
        row = slot - self.START
        self.table.item(row, 2).setText("0")
        self._busy = True
        self.qty_spin.setValue(0)
        self._busy = False
        win = self.window()
        if hasattr(win, "_mark_dirty"):
            win._mark_dirty()


class ItemsTab(ItemCategoryPage):
    """Consumables / field items — save slots 0..47 (Power Charge…TNT Ball).

    Slot 48 is the Booster 01a pack counter and is intentionally NOT shown
    here — it lives on the Card Packs page (pack #0).
    """

    TITLE = "Items"
    START = 0
    END = 48


class WeaponsTab(ItemCategoryPage):
    """Weapons — save slots 49..171 (Short Sword…Glorious Horn)."""

    TITLE = "Weapons"
    START = 49
    END = 172


class ArmorTab(ItemCategoryPage):
    """Armor — save slots 172..248 (Bandanna…Apocalypse)."""

    TITLE = "Armor"
    START = 172
    END = 249


class AccessoriesTab(ItemCategoryPage):
    """Accessories — save slots 249..316 (Power Gem…Dark Power S)."""

    TITLE = "Accessories"
    START = 249
    END = 317


def make_inventory_pages() -> list[tuple[str, ItemCategoryPage]]:
    """(nav_title, page) for the four item subclasses, in game order."""
    return [
        ("Items", ItemsTab()),
        ("Weapons", WeaponsTab()),
        ("Armor", ArmorTab()),
        ("Accessories", AccessoriesTab()),
    ]


class CardsTab(QWidget):
    """Card collection editor — 314 cards, 0-9 each, at payload 0x06A3."""

    def __init__(self) -> None:
        super().__init__()
        self._save = None
        self._busy = False

        root = QVBoxLayout(self)
        root.setContentsMargins(20, 16, 20, 16)
        root.setSpacing(12)

        self.table = QTableWidget(0, 3)
        self.table.setHorizontalHeaderLabels(["Art", "Card", "Copies"])
        self.table.horizontalHeader().setSectionResizeMode(1, QHeaderView.Stretch)
        self.table.setColumnWidth(0, 66)
        self.table.setColumnWidth(2, 90)
        self.table.verticalHeader().setVisible(False)
        self.table.verticalHeader().setDefaultSectionSize(66)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table.setAlternatingRowColors(True)
        self.table.itemSelectionChanged.connect(self._on_selection)
        root.addWidget(self.table)

        row = QHBoxLayout()
        row.setSpacing(8)
        self.sel_label = QLabel("No card selected")
        self.sel_label.setProperty("role", "hint")
        row.addWidget(self.sel_label)
        row.addStretch(1)
        row.addWidget(QLabel("Copies:"))
        self.qty_spin = QSpinBox()
        self.qty_spin.setRange(0, CARD_MAX)
        self.qty_spin.setValue(CARD_MAX)
        self.qty_spin.valueChanged.connect(self._on_qty_edit)
        row.addWidget(self.qty_spin)
        self.apply_btn = QPushButton("Apply")
        self.apply_btn.setProperty("role", "primary")
        self.apply_btn.clicked.connect(self._apply)
        row.addWidget(self.apply_btn)
        max_btn = QPushButton("Set all to 9")
        max_btn.clicked.connect(self._max_all)
        row.addWidget(max_btn)
        root.addLayout(row)

    def bind(self, save) -> None:
        self._save = save
        self._busy = True
        self._fill_table()
        self._busy = False
        self._select_row(0)

    def _fill_table(self) -> None:
        if self._save is None:
            return
        from dmw3editor.ui import assets

        self.table.setRowCount(CARD_COUNT)
        for i in range(CARD_COUNT):
            name = TABLES.card_name(i)
            copies = self._save.card_count(i)
            art = QTableWidgetItem()
            pix = assets.card_image_for_save_index(i)
            if pix is not None:
                art.setData(
                    Qt.DecorationRole,
                    assets.scaled(pix, 58),
                )
                art.setToolTip(name)
            art.setTextAlignment(Qt.AlignCenter)
            self.table.setItem(i, 0, art)
            self.table.setItem(i, 1, QTableWidgetItem(name))
            self.table.setItem(i, 2, QTableWidgetItem(str(copies)))
        self.table.resizeRowsToContents()

    def _selected_index(self) -> int | None:
        """Card save index behind the selected row, or None."""
        if self._save is None:
            return None
        rows = self.table.selectionModel().selectedRows()
        r = rows[0].row() if rows else self.table.currentRow()
        if 0 <= r < self.table.rowCount():
            return r
        return None

    def _select_row(self, row: int) -> None:
        if not 0 <= row < self.table.rowCount():
            self.apply_btn.setEnabled(False)
            self.sel_label.setText("No card selected")
            return
        self.table.selectRow(row)
        self.table.setCurrentCell(row, 1)
        self._sync_selection()

    def _on_selection(self, *_args) -> None:
        if self._busy:
            return
        self._sync_selection()

    def _sync_selection(self) -> None:
        idx = self._selected_index()
        if idx is None:
            self.apply_btn.setEnabled(False)
            self.sel_label.setText("No card selected")
            return
        self.sel_label.setText(TABLES.card_name(idx))
        self.apply_btn.setEnabled(True)
        self._busy = True
        self.qty_spin.setValue(self._save.card_count(idx))
        self._busy = False

    def _on_qty_edit(self, _value: int) -> None:
        if self._busy or self._save is None:
            return
        idx = self._selected_index()
        if idx is None:
            return
        self.table.item(idx, 2).setText(str(self.qty_spin.value()))

    def _apply(self) -> None:
        if self._save is None:
            return
        idx = self._selected_index()
        if idx is None:
            return
        cnt = self.qty_spin.value()
        try:
            self._save.set_card_count(idx, cnt)
        except SaveError:
            return
        self.table.item(idx, 2).setText(str(cnt))
        win = self.window()
        if hasattr(win, "_mark_dirty"):
            win._mark_dirty()

    def _max_all(self) -> None:
        if self._save is None:
            return
        self._save.set_all_cards(CARD_MAX)
        for i in range(CARD_COUNT):
            self.table.item(i, 2).setText(str(CARD_MAX))
        win = self.window()
        if hasattr(win, "_mark_dirty"):
            win._mark_dirty()


class PacksTab(QWidget):
    """Card pack quantities editor — all 35 booster packs (0-99 each).

    Packs are stored in the main item-qty array: Booster 01a at slot 48,
    Booster 02a-15a at 318-331, Booster 1b-15b at 332-346, R-Booster 01-05 at
    347-351. CONFIRMED by the player's in-game item-tab read-back (2026-09-02).
    """

    def __init__(self) -> None:
        super().__init__()
        self._save = None
        self._busy = False

        root = QVBoxLayout(self)
        root.setContentsMargins(20, 16, 20, 16)
        root.setSpacing(12)

        self.table = QTableWidget(0, 3)
        self.table.setHorizontalHeaderLabels(["Art", "Pack", "Qty"])
        self.table.horizontalHeader().setSectionResizeMode(1, QHeaderView.Stretch)
        self.table.setColumnWidth(0, 56)
        self.table.setColumnWidth(2, 80)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table.verticalHeader().setVisible(False)
        self.table.verticalHeader().setDefaultSectionSize(58)
        self.table.itemSelectionChanged.connect(self._on_selection)
        root.addWidget(self.table)

        row = QHBoxLayout()
        row.setSpacing(8)
        self.sel_label = QLabel("No pack selected")
        self.sel_label.setProperty("role", "hint")
        row.addWidget(self.sel_label)
        row.addStretch(1)
        row.addWidget(QLabel("Qty:"))
        self.qty_spin = QSpinBox()
        self.qty_spin.setRange(0, ITEM_MAX)
        self.qty_spin.setValue(ITEM_MAX)
        self.qty_spin.valueChanged.connect(self._on_qty_edit)
        row.addWidget(self.qty_spin)
        self.apply_btn = QPushButton("Apply")
        self.apply_btn.setProperty("role", "primary")
        self.apply_btn.clicked.connect(self._apply)
        row.addWidget(self.apply_btn)
        all_btn = QPushButton("Set all to 99")
        all_btn.clicked.connect(self._max_all)
        row.addWidget(all_btn)
        zero_btn = QPushButton("Zero all")
        zero_btn.clicked.connect(self._zero_all)
        row.addWidget(zero_btn)
        root.addLayout(row)

    def bind(self, save) -> None:
        self._save = save
        self._busy = True
        self._fill_table()
        self._busy = False
        self._select_row(0)

    def _fill_table(self) -> None:
        if self._save is None:
            return
        from dmw3editor.ui import assets

        self.table.setRowCount(PACK_COUNT)
        for i in range(PACK_COUNT):
            slot = TABLES.pack_save_slot(i)
            qty = self._save.item_qty(slot) if slot >= 0 else -1
            art = QTableWidgetItem()
            pix = assets.icon_pixmap("booster_icon")
            if pix is not None:
                art.setData(Qt.DecorationRole, assets.scaled(pix, 40))
                art.setToolTip("Booster pack")
            art.setTextAlignment(Qt.AlignCenter)
            self.table.setItem(i, 0, art)
            self.table.setItem(i, 1, QTableWidgetItem(TABLES.pack_name(i)))
            self.table.setItem(i, 2, QTableWidgetItem(str(qty)))
        self.table.resizeRowsToContents()

    def _selected_pack(self) -> int | None:
        """Pack index behind the selected row, or None."""
        if self._save is None:
            return None
        rows = self.table.selectionModel().selectedRows()
        r = rows[0].row() if rows else self.table.currentRow()
        if 0 <= r < self.table.rowCount():
            return r
        return None

    def _select_row(self, row: int) -> None:
        if not 0 <= row < self.table.rowCount():
            self.apply_btn.setEnabled(False)
            self.sel_label.setText("No pack selected")
            return
        self.table.selectRow(row)
        self.table.setCurrentCell(row, 1)
        self._sync_selection()

    def _on_selection(self, *_args) -> None:
        if self._busy:
            return
        self._sync_selection()

    def _sync_selection(self) -> None:
        idx = self._selected_pack()
        if idx is None:
            self.apply_btn.setEnabled(False)
            self.sel_label.setText("No pack selected")
            return
        self.sel_label.setText(TABLES.pack_name(idx))
        self.apply_btn.setEnabled(True)
        self._busy = True
        slot = TABLES.pack_save_slot(idx)
        self.qty_spin.setValue(self._save.item_qty(slot) if slot >= 0 else 0)
        self._busy = False

    def _on_qty_edit(self, _value: int) -> None:
        if self._busy or self._save is None:
            return
        idx = self._selected_pack()
        if idx is None:
            return
        self.table.item(idx, 2).setText(str(self.qty_spin.value()))

    def _apply(self) -> None:
        if self._save is None:
            return
        idx = self._selected_pack()
        if idx is None:
            return
        slot = TABLES.pack_save_slot(idx)
        qty = self.qty_spin.value()
        try:
            self._save.set_item_qty(slot, qty)
        except SaveError:
            return
        self.table.item(idx, 2).setText(str(qty))
        win = self.window()
        if hasattr(win, "_mark_dirty"):
            win._mark_dirty()

    def _max_all(self) -> None:
        if self._save is None:
            return
        for i in range(PACK_COUNT):
            slot = TABLES.pack_save_slot(i)
            self._save.set_item_qty(slot, ITEM_MAX)
            self.table.item(i, 2).setText(str(ITEM_MAX))
        win = self.window()
        if hasattr(win, "_mark_dirty"):
            win._mark_dirty()

    def _zero_all(self) -> None:
        if self._save is None:
            return
        for i in range(PACK_COUNT):
            slot = TABLES.pack_save_slot(i)
            self._save.set_item_qty(slot, 0)
            self.table.item(i, 2).setText("0")
        win = self.window()
        if hasattr(win, "_mark_dirty"):
            win._mark_dirty()


class DigimonTab(QWidget):
    """Per-Digimon stat editor — 8 roster partners (Kotemon..Patamon).

    Records at payload 0x0A48 + idx*0x3DC. CONFIRMED 2026-09-02 against the
    player's live card: HP/MP/level/EXP/stat edits all displayed in-game.
    """

    def __init__(self) -> None:
        super().__init__()
        self._save = None
        self._busy = False

        root = QVBoxLayout(self)
        root.setContentsMargins(20, 16, 20, 16)
        root.setSpacing(12)

        # digimon selector + main value row
        row0 = QHBoxLayout()
        row0.setSpacing(10)
        self.roster_avatar = QLabel()
        self.roster_avatar.setFixedSize(56, 56)
        self.roster_avatar.setAlignment(Qt.AlignCenter)
        row0.addWidget(self.roster_avatar)
        col = QVBoxLayout()
        col.setSpacing(2)
        col.addWidget(QLabel("Digimon:"))
        self.roster_combo = QComboBox()
        for i, nm in enumerate(DIGI_ROSTER_NAMES):
            self.roster_combo.addItem(nm, i)
        self.roster_combo.currentIndexChanged.connect(self._reload)
        col.addWidget(self.roster_combo)
        row0.addLayout(col)
        row0.addStretch(1)
        max_btn = QPushButton("Max this Digimon")
        max_btn.clicked.connect(self._max_one)
        row0.addWidget(max_btn)
        all_btn = QPushButton("Max all")
        all_btn.clicked.connect(self._max_all)
        row0.addWidget(all_btn)
        root.addLayout(row0)

        # field grid: level, exp, hp, mp + 13 stats
        grid = QGridLayout()
        self._spinboxes = {}

        self.level_spin = QSpinBox()
        self.level_spin.setRange(1, DIGI_MAX_LEVEL)
        grid.addWidget(QLabel("Level:"), 0, 0)
        grid.addWidget(self.level_spin, 0, 1)

        self.exp_spin = QSpinBox()
        self.exp_spin.setRange(0, DIGI_MAX_EXP)
        self.exp_spin.setGroupSeparatorShown(True)
        grid.addWidget(QLabel("EXP:"), 0, 2)
        grid.addWidget(self.exp_spin, 0, 3)

        self.hp_spin = QSpinBox()
        self.hp_spin.setRange(0, DIGI_MAX_HP)
        grid.addWidget(QLabel("HP (cur+max):"), 1, 0)
        grid.addWidget(self.hp_spin, 1, 1)

        self.mp_spin = QSpinBox()
        self.mp_spin.setRange(0, DIGI_MAX_MP)
        grid.addWidget(QLabel("MP (cur+max):"), 1, 2)
        grid.addWidget(self.mp_spin, 1, 3)

        # 13 stats -> 4 rows x (label, spin)
        for si, sname in enumerate(DIGI_STAT_NAMES):
            r = 2 + si // 4
            c = (si % 4) * 2
            spin = QSpinBox()
            spin.setRange(0, DIGI_MAX_STAT)
            self._spinboxes[si] = spin
            grid.addWidget(QLabel(sname + ":"), r, c)
            grid.addWidget(spin, r, c + 1)
        root.addLayout(grid)

        row1 = QHBoxLayout()
        apply_btn = QPushButton("Apply to save")
        apply_btn.clicked.connect(self._apply)
        row1.addWidget(apply_btn)
        self.status_label = QLabel("")
        self.status_label.setProperty("role", "hint")
        row1.addWidget(self.status_label)
        row1.addStretch(1)
        root.addLayout(row1)
        root.addStretch(1)

    # -- public -----------------------------------------------------------
    def bind(self, save) -> None:
        self._save = save
        self._busy = True
        self._reload()
        self._busy = False

    # -- internal ---------------------------------------------------------
    def _reload(self) -> None:
        if self._save is None:
            return
        idx = self.roster_combo.currentData()
        st = self._save.digimon_stats(idx)
        self._busy = True
        self._update_avatar(idx)
        self.level_spin.setValue(st["level"])
        self.exp_spin.setValue(st["exp"])
        self.hp_spin.setValue(st["hp"])
        self.mp_spin.setValue(st["mp"])
        for si, spin in self._spinboxes.items():
            spin.setValue(st["stats"][si])
        self.status_label.setText(
            f"{self.roster_combo.currentText()} — HP {st['hp']}/{st['hp_max']}, "
            f"MP {st['mp']}/{st['mp_max']}, Lv {st['level']}"
        )
        self._busy = False

    def _update_avatar(self, roster_index: int) -> None:
        """Show the roster partner's portrait next to the selector."""
        from dmw3editor.ui import assets

        if not 0 <= roster_index < len(assets.ROSTER_NAMES):
            self.roster_avatar.setPixmap(QPixmap())
            return
        pix = assets.roster_pixmap(assets.ROSTER_NAMES[roster_index])
        self.roster_avatar.setPixmap(
            assets.scaled(pix, 52) if pix else QPixmap()
        )

    def _apply(self) -> None:
        if self._save is None:
            return
        idx = self.roster_combo.currentData()
        try:
            self._save.set_digimon_level(idx, self.level_spin.value())
            self._save.set_digimon_exp(idx, self.exp_spin.value())
            self._save.set_digimon_hp(idx, self.hp_spin.value())
            self._save.set_digimon_mp(idx, self.mp_spin.value())
            for si, spin in self._spinboxes.items():
                self._save.set_digimon_stat(idx, si, spin.value())
        except SaveError as exc:
            self.status_label.setText(f"Error: {exc}")
            return
        self.status_label.setText(
            f"{self.roster_combo.currentText()} applied ✓"
        )
        win = self.window()
        if hasattr(win, "_mark_dirty"):
            win._mark_dirty()

    def _max_one(self) -> None:
        if self._save is None:
            return
        idx = self.roster_combo.currentData()
        self._save.set_digimon_maxed(idx)
        self._reload()
        win = self.window()
        if hasattr(win, "_mark_dirty"):
            win._mark_dirty()

    def _max_all(self) -> None:
        if self._save is None:
            return
        for idx in range(DIGI_STAT_COUNT):
            self._save.set_digimon_maxed(idx)
        self._reload()
        win = self.window()
        if hasattr(win, "_mark_dirty"):
            win._mark_dirty()


class KeyItemsTab(QWidget):
    """Key/important item flags editor.

    48 flags in Important-screen order (payload 0x0380..0x03A6, Monmon DDNA
    at main-array slot 317, and 0x0507..0x050E). CONFIRMED 2026-09-02
    byte-exact against the player's in-game Important screen (35 owned).
    """

    def __init__(self) -> None:
        super().__init__()
        self._save = None
        self._busy = False
        self._region = "USA"

        root = QVBoxLayout(self)
        root.setContentsMargins(20, 16, 20, 16)
        root.setSpacing(12)

        self.table = QTableWidget(0, 3)
        self.table.setHorizontalHeaderLabels(["Art", "Important Item", "Owned"])
        self.table.horizontalHeader().setSectionResizeMode(1, QHeaderView.Stretch)
        self.table.setColumnWidth(0, 56)
        self.table.setColumnWidth(2, 80)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table.verticalHeader().setVisible(False)
        self.table.verticalHeader().setDefaultSectionSize(58)
        self.table.itemSelectionChanged.connect(self._on_selection)
        root.addWidget(self.table)

        row = QHBoxLayout()
        row.setSpacing(8)
        self.sel_label = QLabel("No item selected")
        self.sel_label.setProperty("role", "hint")
        row.addWidget(self.sel_label)
        row.addStretch(1)
        row.addWidget(QLabel("Owned:"))
        self.qty_spin = QSpinBox()
        self.qty_spin.setRange(0, 1)
        self.qty_spin.setValue(1)
        self.qty_spin.valueChanged.connect(self._on_qty_edit)
        row.addWidget(self.qty_spin)
        self.apply_btn = QPushButton("Apply")
        self.apply_btn.setProperty("role", "primary")
        self.apply_btn.clicked.connect(self._apply)
        row.addWidget(self.apply_btn)
        all_btn = QPushButton("Give all")
        all_btn.clicked.connect(self._all)
        row.addWidget(all_btn)
        clear_btn = QPushButton("Clear all")
        clear_btn.clicked.connect(lambda: self._all(0))
        row.addWidget(clear_btn)
        root.addLayout(row)

    def bind(self, save) -> None:
        self._save = save
        self._region = save.region_guess if hasattr(save, "region_guess") else "USA"
        self._busy = True
        self._fill_table()
        self._busy = False
        self._select_row(0)

    def _fill_table(self) -> None:
        if self._save is None:
            return
        from dmw3editor.ui import assets

        is_eur = self._region == "EUR"
        self.table.setRowCount(48)
        for i in range(48):
            name = TABLES.key_item_name(i, self._region)
            val = self._save.key_item_count(i)
            art = QTableWidgetItem()
            pix = assets.icon_pixmap("important_items_icon")
            if pix is not None:
                art.setData(Qt.DecorationRole, assets.scaled(pix, 40))
                art.setToolTip(name)
            art.setTextAlignment(Qt.AlignCenter)
            self.table.setItem(i, 0, art)
            cell = QTableWidgetItem(name)
            if is_eur and TABLES.key_item_jp_in_eur(i):
                cell.setForeground(QColor("#e6b450"))
                cell.setToolTip(
                    "Untranslated in the EUR ROM: this item has no English "
                    "string, so the game shows Japanese text for it."
                )
            elif is_eur and TABLES.key_item_regions_differ(i):
                cell.setForeground(QColor("#7fb3e6"))
                cell.setToolTip(
                    "Name differs between USA and EUR localizations "
                    f"(USA: {TABLES.key_item_name(i, 'USA')})."
                )
            self.table.setItem(i, 1, cell)
            self.table.setItem(i, 2, QTableWidgetItem(str(val)))
        self.table.resizeRowsToContents()

    def _selected_item(self) -> int | None:
        """Key-item index behind the selected row, or None."""
        if self._save is None:
            return None
        rows = self.table.selectionModel().selectedRows()
        r = rows[0].row() if rows else self.table.currentRow()
        if 0 <= r < self.table.rowCount():
            return r
        return None

    def _select_row(self, row: int) -> None:
        if not 0 <= row < self.table.rowCount():
            self.apply_btn.setEnabled(False)
            self.sel_label.setText("No item selected")
            return
        self.table.selectRow(row)
        self.table.setCurrentCell(row, 1)
        self._sync_selection()

    def _on_selection(self, *_args) -> None:
        if self._busy:
            return
        self._sync_selection()

    def _sync_selection(self) -> None:
        idx = self._selected_item()
        if idx is None:
            self.apply_btn.setEnabled(False)
            self.sel_label.setText("No item selected")
            return
        self.sel_label.setText(TABLES.key_item_name(idx, self._region))
        self.apply_btn.setEnabled(True)
        self._busy = True
        self.qty_spin.setValue(self._save.key_item_count(idx))
        self._busy = False

    def _on_qty_edit(self, _value: int) -> None:
        if self._busy or self._save is None:
            return
        idx = self._selected_item()
        if idx is None:
            return
        self.table.item(idx, 2).setText(str(self.qty_spin.value()))

    def _apply(self) -> None:
        if self._save is None:
            return
        idx = self._selected_item()
        if idx is None:
            return
        val = self.qty_spin.value()
        try:
            self._save.set_key_item(idx, val)
        except SaveError:
            return
        self.table.item(idx, 2).setText(str(val))
        win = self.window()
        if hasattr(win, "_mark_dirty"):
            win._mark_dirty()

    def _all(self, val: int = 1) -> None:
        if self._save is None:
            return
        self._save.set_all_key_items(val)
        for i in range(48):
            self.table.item(i, 2).setText(str(val))
        win = self.window()
        if hasattr(win, "_mark_dirty"):
            win._mark_dirty()
# Canonical order of the 44 evolved forms (decomp DigimonId 9..52). Every one
# of the 8 rookies can reach every one of these (STFGTREP EVOS tables list the
# same 44 destinations for each rookie — verified from the decomp).
EVOLVED_FORMS_44 = [
    "Dinohumon", "Hookmon", "Grizzmon", "Greymon", "ExVeemon", "Growlmon",
    "Kyubimon", "Angemon", "Devimon", "Stingmon", "Angewomon", "Kyukimon",
    "Armormon", "GrapLeomon", "MetalGreymon", "SkullGreymon", "Paildramon",
    "WarGrowlmon", "Taomon", "MagnaAngemon", "Myotismon", "MetalMamemon",
    "Kabuterimon", "Digitamamon", "GuardiAngemon", "Cannondramon", "Marsmon",
    "WarGreymon", "Imperialdramon", "Gallantmon", "Sakuyamon", "Seraphimon",
    "MetalGarurumon", "Rosemon", "BKWarGreymon", "ImperialdramonFM",
    "MaloMyotismon", "MegaGargomon", "GranKuwagamon", "Phoenixmon", "Omnimon",
    "ImperialdramonPM", "Beelzemon", "Diaboromon",
]


class DigivolutionTab(QWidget):
    """Per-digimon digivolution DV levels.

    Each of the 8 roster records stores DV levels for digivolution forms: a
    header/primary form at record+0x72 and 43 data slots at record+0x74+20k
    with the level u16 at slot+18. CONFIRMED 2026-09-02 by a live probe:
    writing slot+18..19 to 0x51..0x5A made the in-game DV screen show those
    forms at 81..90 in exact slot order.

    Every digimon can reach all 44 evolved forms (decomp EVOS tables). Which
    forms it has earned is visible in the slot's identity marker u16 at
    slot+16..17 (each form has a constant card-independent marker; marker 0 =
    not earned — probe 2 proved a level alone does NOT earn/display a form).
    The Earned checkbox mirrors that marker.
    """

    def __init__(self) -> None:
        super().__init__()
        self._save = None
        self._busy = False

        root = QVBoxLayout(self)
        root.setContentsMargins(20, 16, 20, 16)
        root.setSpacing(12)

        # 8 sprite tiles (one per rookie) — keeps the GUI's roster theme.
        from dmw3editor.ui import assets

        self._digi_buttons: list[QToolButton] = []
        self._digi_group = QButtonGroup(self)
        self._digi_group.setExclusive(True)
        row0 = QHBoxLayout()
        row0.setSpacing(6)
        for i, nm in enumerate(DIGI_ROSTER_NAMES):
            btn = QToolButton()
            btn.setText(nm)
            btn.setCheckable(True)
            btn.setProperty("role", "sprite")
            btn.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)
            btn.setIconSize(QSize(42, 42))
            pix = assets.roster_pixmap(nm)
            if pix is None:
                pix = QPixmap()
            btn.setIcon(QIcon(assets.scaled(pix, 38)))
            btn.setCursor(Qt.PointingHandCursor)
            btn.setToolTip(f"{nm} digivolutions")
            self._digi_group.addButton(btn, i)
            btn.clicked.connect(self._reload)
            row0.addWidget(btn)
            self._digi_buttons.append(btn)
        self._digi_buttons[0].setChecked(True)
        row0.addStretch(1)
        max_all = QPushButton("Max earned DV to 99")
        max_all.clicked.connect(self._max_earned)
        row0.addWidget(max_all)
        root.addLayout(row0)

        self.table = QTableWidget(0, 3)
        self.table.setHorizontalHeaderLabels(["Earned", "Digivolution", "DV Level"])
        self.table.horizontalHeader().setSectionResizeMode(1, QHeaderView.Stretch)
        self.table.setColumnWidth(0, 64)
        self.table.setColumnWidth(2, 100)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table.verticalHeader().setVisible(False)
        root.addWidget(self.table, 1)

        row = QHBoxLayout()
        row.setSpacing(8)
        apply_btn = QPushButton("Apply all DV levels")
        apply_btn.clicked.connect(self._apply_all)
        row.addWidget(apply_btn)
        self.status_label = QLabel("")
        self.status_label.setProperty("role", "hint")
        row.addWidget(self.status_label)
        row.addStretch(1)
        root.addLayout(row)
        self._rows = []  # (name, is_header, slot_index, checkbox, spin)

    # -- public -----------------------------------------------------------
    def bind(self, save) -> None:
        self._save = save
        self._busy = True
        self._reload()
        self._busy = False

    def select_roster(self, name: str) -> None:
        """Select a rookie by name (sprite tile highlight + reload)."""
        if name not in DIGI_ROSTER_NAMES:
            return
        idx = DIGI_ROSTER_NAMES.index(name)
        self._digi_buttons[idx].setChecked(True)
        self._reload()

    def selected_roster(self) -> str:
        bid = self._digi_group.checkedId()
        if 0 <= bid < len(DIGI_ROSTER_NAMES):
            return DIGI_ROSTER_NAMES[bid]
        return DIGI_ROSTER_NAMES[0]

    # -- internal ---------------------------------------------------------
    def _reload(self, *_args) -> None:
        if self._save is None:
            return
        from PySide6.QtWidgets import QCheckBox

        roster_name = self.selected_roster()
        roster_idx = DIGI_ROSTER_NAMES.index(roster_name)
        dv = self._save.digimon_dv(roster_idx)
        header_form = D_DV_HEADER_FORMS.get(roster_name, "")
        # Each data slot's identity is its marker (u16 @ +16..17); reverse-map
        # to the form name. Marker 0 = slot never earned. The header form
        # (row 1) is present whenever its DV level is stored (>0).
        earned = []
        if header_form and dv["header"] > 0:
            earned.append(header_form)
        slot_of: dict[str, int] = {}
        for si in range(D_DV_COUNT):
            marker = dv["markers"][si]
            if marker == 0:
                continue
            nm = DV_FORM_MARKER_NAMES.get(marker)
            if nm is None:
                nm = f"UnknownForm(0x{marker:04x})"
            if nm not in slot_of:
                slot_of[nm] = si
                earned.append(nm)
        order = list(earned)
        for nm in EVOLVED_FORMS_44:
            if nm not in earned:
                order.append(nm)

        self._busy = True
        self.table.setRowCount(len(order))
        self._rows = []
        empty_left = sum(1 for m in dv["markers"] if m == 0)
        for ri, nm in enumerate(order):
            is_earned = nm in earned
            check = QCheckBox()
            check.setProperty("role", "tick")
            check.setChecked(is_earned)
            # Earned rows are locked; unearned rows can be ticked to force-earn
            # (writes the form's identity marker into the next empty slot on
            # Apply — CONFIRMED in-game 2026-09-02: Seraphimon 42 / Rosemon 7
            # appeared on Agumon's DV screen after marker+level were written).
            if is_earned:
                check.setEnabled(False)
            else:
                check.setEnabled(empty_left > 0)
                check.setToolTip(
                    "Tick + Apply to force-earn this digivolution at the "
                    "chosen DV level" if empty_left > 0
                    else "No empty DV slots left for this digimon"
                )
            # center the compact tick in its cell (no text label)
            holder = QWidget()
            holder.setStyleSheet("background: transparent;")
            holder.setToolTip(check.toolTip())
            h = QHBoxLayout(holder)
            h.setContentsMargins(0, 0, 0, 0)
            h.setAlignment(Qt.AlignCenter)
            h.addWidget(check)
            self.table.setCellWidget(ri, 0, holder)
            name_item = QTableWidgetItem(nm)
            if not is_earned:
                name_item.setForeground(QColor("#6b7280"))
            self.table.setItem(ri, 1, name_item)
            spin = QSpinBox()
            spin.setRange(0, D_DV_MAX)
            spin.setEnabled(is_earned)
            if is_earned:
                spin.setValue(dv["header"] if nm == header_form else dv["slots"][slot_of[nm]])
                spin.valueChanged.connect(self._on_edit)
            else:
                spin.setValue(1)
                check.toggled.connect(
                    lambda ck, sp=spin: self._on_earn_toggle(ck, sp)
                )
            self.table.setCellWidget(ri, 2, spin)
            self._rows.append((nm, nm == header_form, slot_of.get(nm), check, spin))
        self.table.resizeRowsToContents()
        self.status_label.setText("")
        self._busy = False

    def _on_earn_toggle(self, checked: bool, spin) -> None:
        if self._busy:
            return
        spin.setEnabled(checked)
        if checked:
            spin.setValue(1)
        self.status_label.setText(
            "Earn toggled — set the DV level, then Apply to force-earn" if checked
            else "Earn unticked — Apply will leave this form unearned"
        )

    def _on_edit(self, _v: int) -> None:
        if self._busy:
            return
        self.status_label.setText("DV level changed — Apply to write to the card")

    def _apply_all(self) -> None:
        if self._save is None:
            return
        roster_idx = DIGI_ROSTER_NAMES.index(self.selected_roster())
        earned_now = 0
        try:
            for nm, is_header, slot_idx, check, spin in self._rows:
                if is_header:
                    if spin.isEnabled() and spin.value() >= 1:
                        self._save.set_digimon_dv_header(roster_idx, spin.value())
                    continue
                if check.isChecked() and slot_idx is None:
                    # force-earn into the next empty slot at the chosen level
                    self._save.earn_digimon_dv(roster_idx, nm, spin.value())
                    earned_now += 1
                    continue
                if slot_idx is not None and spin.isEnabled():
                    val = spin.value()
                    if val >= 1:
                        self._save.set_digimon_dv_slot(roster_idx, slot_idx, val)
        except SaveError as exc:
            self.status_label.setText(f"Error: {exc}")
            return
        if earned_now:
            self.status_label.setText(
                f"{self.selected_roster()}: {earned_now} form(s) force-earned ✓"
            )
            self._reload()
        else:
            self.status_label.setText(
                f"{self.selected_roster()} DV levels applied ✓"
            )
        win = self.window()
        if hasattr(win, "_mark_dirty"):
            win._mark_dirty()

    def _max_earned(self) -> None:
        if self._save is None:
            return
        for nm, is_header, slot_idx, _check, spin in self._rows:
            if spin.isEnabled():
                spin.setValue(D_DV_MAX)
        self._apply_all()
