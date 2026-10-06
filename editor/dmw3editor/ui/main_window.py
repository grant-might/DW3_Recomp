"""DMW3 Save Editor, main window.

ShadCN-style single-window app: a left sidebar groups the editor into pages
(Save slots, Inventory subclasses, Card Packs, Digimon, Cards, Important
Items, Field locator, Hex editor) rendered in one stacked content area.

UX principles applied here:
  * Nothing is editable until a save is loaded.
  * Every edit is staged in memory; the file changes only on Save/Save As.
  * A backup (.bak) is written before the first overwrite of any file.
  * Unproven regions are not presented as editable fields; the hex editor
    (Developer mode) marks them read-only rather than pretending we
    understand them.
"""

from __future__ import annotations

import pathlib
import shutil
import sys

from PySide6.QtCore import Qt, Signal
from PySide6.QtGui import (
    QAction,
    QIcon,
    QKeySequence,
    QPainter,
    QPainterPath,
    QPixmap,
)
from PySide6.QtWidgets import (
    QApplication,
    QButtonGroup,
    QCheckBox,
    QComboBox,
    QFileDialog,
    QFormLayout,
    QFrame,
    QGridLayout,
    QGroupBox,
    QHBoxLayout,
    QLabel,
    QMainWindow,
    QMessageBox,
    QPushButton,
    QScrollArea,
    QSpinBox,
    QStackedWidget,
    QStatusBar,
    QVBoxLayout,
    QWidget,
)

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[2]))

from dmw3editor.core import memcard as mc
from dmw3editor.core.save import (
    DMW3Save,
    HOURS_MAX,
    LEVEL_MAX,
    MONEY_MAX,
    PARTY_SIZE,
    SLOT_OFFSETS,
    TABLES,
    SaveError,
    party_min_for_region,
)
from dmw3editor.paths import repo_root
from dmw3editor.ui import theme
from dmw3editor.ui.collections import (
    CardsTab,
    DigimonTab,
    DigivolutionTab,
    KeyItemsTab,
    PacksTab,
    make_inventory_pages,
)
from dmw3editor.ui.field_locator import FieldLocator
from dmw3editor.ui.hex_editor import HexEditorPage
from dmw3editor.ui.themes_tab import ThemesTab, saved_theme

APP_NAME = "Digimon World 3 Save Editor"
FILTER = (
    "PS1 memory cards (*.mcr *.mcd *.mc *.gme *.bin *.vgs *.vmp *.srm *.ps);;"
    "All files (*)"
)

SIDEBAR_WIDTH = 216


def _logo_path() -> pathlib.Path:
    """logo.png lives at the repo/bundle root in both dev and frozen layouts."""
    return repo_root() / "logo.png"


def _logo_pixmap(max_height: int = 64, radius: int = 14) -> QPixmap:
    """Load the app logo (repo root logo.png) scaled and rounded."""
    logo_path = _logo_path()
    if not logo_path.exists():
        return QPixmap()
    pm = QPixmap(str(logo_path))
    if pm.isNull():
        return QPixmap()
    pm = pm.scaledToHeight(max_height, Qt.SmoothTransformation)
    out = QPixmap(pm.size())
    out.fill(Qt.transparent)
    p = QPainter(out)
    p.setRenderHint(QPainter.Antialiasing)
    clip = QPainterPath()
    clip.addRoundedRect(0, 0, out.width(), out.height(), radius, radius)
    p.setClipPath(clip)
    p.drawPixmap(0, 0, pm)
    p.end()
    return out


class SlotEditor(QWidget):
    """Editor for one in-game save slot."""

    changed = Signal()

    def __init__(self, index: int) -> None:
        super().__init__()
        self.index = index
        self._loading = False
        self._region = "USA"
        self._save: DMW3Save | None = None

        root = QVBoxLayout(self)
        root.setContentsMargins(20, 16, 20, 16)
        root.setSpacing(14)

        self.empty_note = QLabel(
            "This slot is empty. Editing it would create a save the game may "
            "not accept, load a card whose slot is in use instead."
        )
        self.empty_note.setWordWrap(True)
        self.empty_note.setProperty("role", "warning")
        self.empty_note.hide()
        root.addWidget(self.empty_note)

        # --- party -------------------------------------------------------
        # Untitled card: the avatar/digimon/level rows are self-explanatory
        # (the old "Party: Digimon and levels" group title was removed
        # 2026-09-03 per user request).
        party_box = QFrame()
        party_box.setProperty("role", "card")
        grid = QGridLayout(party_box)
        grid.setContentsMargins(16, 16, 16, 16)
        grid.setHorizontalSpacing(12)
        grid.setVerticalSpacing(10)
        grid.addWidget(QLabel("Slot"), 0, 0)
        grid.addWidget(QLabel("Digimon (base rookies only)"), 0, 1)
        grid.addWidget(QLabel("Level"), 0, 2)

        self.party_ids: list[QComboBox] = []
        self.party_levels: list[QSpinBox] = []
        self.party_avatars: list[QLabel] = []
        for i in range(PARTY_SIZE):
            grid.addWidget(QLabel(f"{i + 1}"), i + 1, 0)

            row_wrap = QWidget()
            row_wrap.setStyleSheet("background: transparent;")
            wrap_lay = QHBoxLayout(row_wrap)
            wrap_lay.setContentsMargins(0, 0, 0, 0)
            wrap_lay.setSpacing(8)
            avatar = QLabel()
            avatar.setFixedSize(34, 34)
            avatar.setAlignment(Qt.AlignCenter)
            wrap_lay.addWidget(avatar)
            combo = QComboBox()
            combo.setEditable(False)
            combo.currentIndexChanged.connect(self._on_combo_changed)
            wrap_lay.addWidget(combo, 1)
            grid.addWidget(row_wrap, i + 1, 1)
            self.party_ids.append(combo)
            self.party_avatars.append(avatar)

            lv = QSpinBox()
            lv.setRange(1, LEVEL_MAX)
            lv.valueChanged.connect(self._on_edit)
            grid.addWidget(lv, i + 1, 2)
            self.party_levels.append(lv)

        grid.setColumnStretch(1, 1)
        max_all = QPushButton(f"Set all to Lv{LEVEL_MAX}")
        max_all.setProperty("role", "ghost")
        max_all.clicked.connect(self._max_levels)
        grid.addWidget(max_all, PARTY_SIZE + 1, 2)
        root.addWidget(party_box)

        # --- player ------------------------------------------------------
        player_box = QGroupBox("Player")
        form = QFormLayout(player_box)
        form.setContentsMargins(16, 20, 16, 16)
        form.setSpacing(10)
        form.setLabelAlignment(Qt.AlignRight | Qt.AlignVCenter)
        form.setFormAlignment(Qt.AlignLeft | Qt.AlignTop)

        self.partner = QComboBox()
        self.partner.currentIndexChanged.connect(self._on_edit)
        form.addRow("Partner", self.partner)

        money_row = QHBoxLayout()
        money_row.setSpacing(10)
        self.money = QSpinBox()
        self.money.setRange(0, MONEY_MAX)
        self.money.setGroupSeparatorShown(True)
        self.money.valueChanged.connect(self._on_edit)
        money_row.addWidget(self.money, 1)
        money_max = QPushButton("Max")
        money_max.setProperty("role", "ghost")
        money_max.clicked.connect(lambda: self.money.setValue(MONEY_MAX))
        money_row.addWidget(money_max)
        money_row.setContentsMargins(0, 0, 0, 0)
        holder = QWidget()
        holder.setLayout(money_row)
        holder.setStyleSheet("background: transparent;")
        form.addRow("Bits", holder)

        time_row = QHBoxLayout()
        time_row.setSpacing(10)
        self.hours = QSpinBox()
        self.hours.setRange(0, HOURS_MAX)
        self.hours.setSuffix(" h")
        self.minutes = QSpinBox()
        self.minutes.setRange(0, 59)
        self.minutes.setSuffix(" m")
        self.seconds = QSpinBox()
        self.seconds.setRange(0, 59)
        self.seconds.setSuffix(" s")
        for w in (self.hours, self.minutes, self.seconds):
            w.valueChanged.connect(self._on_edit)
            w.setFixedWidth(84)
            time_row.addWidget(w)
        time_row.addStretch(1)
        time_row.setContentsMargins(0, 0, 0, 0)
        time_holder = QWidget()
        time_holder.setLayout(time_row)
        time_holder.setStyleSheet("background: transparent;")
        form.addRow("Play time", time_holder)
        root.addWidget(player_box)

        root.addStretch(1)

    # -- data binding -----------------------------------------------------
    def _populate_rookies(self, region: str) -> None:
        """Fill party/partner combos with the 8 rookies for a region."""
        choices = TABLES.base_rookie_choices(region)
        for combo in [*self.party_ids, self.partner]:
            combo.blockSignals(True)
            combo.clear()
            for did, name in choices:
                combo.addItem(f"{name}  (#{did})", did)
            combo.blockSignals(False)

    def _update_avatars(self) -> None:
        """Show the selected rookie's portrait in each party slot."""
        from dmw3editor.ui import assets

        for i, combo in enumerate(self.party_ids):
            did = combo.currentData()
            if did is None:
                continue
            pmin = party_min_for_region(self._region)
            idx = did - pmin
            if not 0 <= idx < len(assets.ROSTER_NAMES):
                self.party_avatars[i].setPixmap(QPixmap())
                continue
            pix = assets.roster_pixmap(assets.ROSTER_NAMES[idx])
            self.party_avatars[i].setPixmap(
                assets.scaled(pix, 32) if pix else QPixmap()
            )

    def bind(self, save: DMW3Save) -> None:
        self._save = save
        self._region = save.region_guess
        slot = save.slot(self.index)
        self._loading = True
        try:
            self.empty_note.setVisible(slot.is_empty)
            self._populate_rookies(save.region_guess)
            for i, (did, level) in enumerate(slot.party):
                idx = self.party_ids[i].findData(did)
                self.party_ids[i].setCurrentIndex(max(idx, 0))
                self.party_levels[i].setValue(max(1, min(level, LEVEL_MAX)))
            pidx = self.partner.findData(slot.partner_id)
            self.partner.setCurrentIndex(max(pidx, 0))
            self.money.setValue(min(slot.money, MONEY_MAX))
            h, m, s = slot.play_time
            self.hours.setValue(min(h, HOURS_MAX))
            self.minutes.setValue(min(m, 59))
            self.seconds.setValue(min(s, 59))
            self._update_avatars()
        finally:
            self._loading = False

    def _on_combo_changed(self, *_args) -> None:
        if not self._loading:
            self._update_avatars()
        self._on_edit(*_args)

    def _max_levels(self) -> None:
        for w in self.party_levels:
            w.setValue(LEVEL_MAX)

    def _on_edit(self, *_args) -> None:
        if self._loading or self._save is None:
            return
        try:
            for i in range(PARTY_SIZE):
                self._save.set_party_member(
                    self.index,
                    i,
                    self.party_ids[i].currentData(),
                    self.party_levels[i].value(),
                )
            self._save.set_partner(self.index, self.partner.currentData())
            self._save.set_money(self.index, self.money.value())
            self._save.set_play_time(
                self.index,
                self.hours.value(),
                self.minutes.value(),
                self.seconds.value(),
            )
        except SaveError as exc:
            QMessageBox.warning(self, "Invalid value", str(exc))
            return
        self.changed.emit()


class MainWindow(QMainWindow):
    def __init__(self) -> None:
        super().__init__()
        self.setWindowTitle(APP_NAME)
        self.resize(1180, 820)
        self.setMinimumSize(980, 640)

        self._card: mc.MemoryCard | None = None
        self._loc = None
        self._save: DMW3Save | None = None
        self._path: pathlib.Path | None = None
        self._dirty = False

        central = QWidget()
        self.setCentralWidget(central)
        outer = QVBoxLayout(central)
        outer.setContentsMargins(0, 0, 0, 0)
        outer.setSpacing(0)

        # header -----------------------------------------------------------
        header = QFrame()
        header.setProperty("role", "header")
        hl = QHBoxLayout(header)
        hl.setContentsMargins(16, 10, 20, 10)
        hl.setSpacing(14)

        # app logo (repo root logo.png), rounded; doubles as the window icon
        logo_pm = _logo_pixmap(56, 12)
        if not logo_pm.isNull():
            logo_lbl = QLabel()
            logo_lbl.setPixmap(logo_pm)
            logo_lbl.setFixedSize(logo_pm.size())
            logo_lbl.setToolTip(APP_NAME)
            hl.addWidget(logo_lbl)
            self.setWindowIcon(QIcon(str(_logo_path())))

        title_col = QVBoxLayout()
        title_col.setSpacing(1)
        title = QLabel(APP_NAME)
        title.setProperty("role", "title")
        title_col.addWidget(title)
        self.subtitle = QLabel("No card loaded")
        self.subtitle.setProperty("role", "subtitle")
        title_col.addWidget(self.subtitle)
        hl.addLayout(title_col, 1)

        # ROM region: auto-detected from the card's version field; the combo
        # lets the user force USA/EUR for unusual cards (re-binds all pages).
        region_box = QWidget()
        region_lay = QVBoxLayout(region_box)
        region_lay.setContentsMargins(0, 0, 0, 0)
        region_lay.setSpacing(1)
        region_cap = QLabel("ROM region")
        region_cap.setProperty("role", "caption")
        region_lay.addWidget(region_cap)
        self.region_combo = QComboBox()
        self.region_combo.addItem("Auto (detected)", "auto")
        self.region_combo.addItem("USA", "USA")
        self.region_combo.addItem("EUR", "EUR")
        self.region_combo.setToolTip(
            "Auto-detected from the card's save version (v3 = USA, v4 = EUR). "
            "Use a manual choice only when a card's version field is missing "
            "or you need to re-interpret it."
        )
        self.region_combo.setMinimumWidth(120)
        self.region_combo.currentIndexChanged.connect(self._on_region_changed)
        region_lay.addWidget(self.region_combo)
        hl.addWidget(region_box)

        self.dev_toggle = QCheckBox("Developer mode")
        self.dev_toggle.setToolTip(
            "Enables the hex editor and other low-level tools. For advanced "
            "users only, the editor still guards unverified regions."
        )
        self.dev_toggle.toggled.connect(self._on_dev_toggle)
        hl.addWidget(self.dev_toggle)

        self.open_btn = QPushButton("Open card…")
        self.open_btn.setProperty("role", "primary")
        self.open_btn.setMinimumWidth(120)
        self.open_btn.clicked.connect(self.open_file)
        hl.addWidget(self.open_btn)
        self.save_btn = QPushButton("Save")
        self.save_btn.setMinimumWidth(110)
        self.save_btn.setProperty("role", "primary")
        self.save_btn.clicked.connect(self.save_file)
        self.save_btn.setEnabled(False)
        hl.addWidget(self.save_btn)
        outer.addWidget(header)

        # body: sidebar + stacked pages ------------------------------------
        body = QHBoxLayout()
        body.setContentsMargins(0, 0, 0, 0)
        body.setSpacing(0)

        sidebar = self._build_sidebar()
        body.addWidget(sidebar)

        self.stack = QStackedWidget()
        self._nav_buttons: list[QPushButton] = []
        self._nav_group = QButtonGroup(self)
        self._nav_group.setExclusive(True)
        self._build_pages()
        self.stack.setEnabled(False)
        body.addWidget(self.stack, 1)
        outer.addLayout(body, 1)

        self.setStatusBar(QStatusBar())
        self.statusBar().showMessage("Open a PS1 memory card to begin.")
        self._build_menu()
        self._sync_enabled()

    # -- sidebar -----------------------------------------------------------
    def _build_sidebar(self) -> QWidget:
        panel = QFrame()
        panel.setObjectName("sidebar")
        panel.setFixedWidth(SIDEBAR_WIDTH)
        panel.setStyleSheet(
            "QFrame#sidebar { background: #101114; border-right: 1px solid #23252b; }"
        )

        scroll = QScrollArea()
        scroll.setWidgetResizable(True)
        scroll.setFrameShape(QFrame.NoFrame)
        scroll.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        content = QWidget()
        content.setObjectName("sidecontent")
        content.setStyleSheet(
            "QWidget#sidecontent { background: transparent; }"
        )
        lay = QVBoxLayout(content)
        lay.setContentsMargins(10, 14, 10, 14)
        lay.setSpacing(2)

        self.sidebar_layout = lay
        self._sidebar_sections = []

        scroll.setWidget(content)
        outer = QVBoxLayout(panel)
        outer.setContentsMargins(0, 0, 0, 0)
        outer.addWidget(scroll)
        return panel

    def _add_section(self, title: str) -> None:
        lbl = QLabel(title)
        lbl.setProperty("role", "section")
        lbl.setContentsMargins(8, 10, 0, 4)
        self.sidebar_layout.addWidget(lbl)

    def _add_nav(self, key: str, title: str) -> None:
        btn = QPushButton(title)
        btn.setProperty("role", "nav")
        btn.setCheckable(True)
        btn.setCursor(Qt.PointingHandCursor)
        btn.clicked.connect(lambda: self._select_page(key))
        self.sidebar_layout.addWidget(btn)
        self._nav_buttons.append(btn)
        self._nav_group.addButton(btn)
    def _build_pages(self) -> None:
        # Pages are registered in this exact order; _keys parallels _nav_buttons.
        self._keys: list[str] = []

        def nav(key: str, label: str) -> None:
            self._keys.append(key)
            self._add_nav(key, label)

        # Save slots -------------------------------------------------------
        self._add_section("SAVE SLOTS")
        self.slot_editors = []
        for i in range(len(SLOT_OFFSETS)):
            nav(f"slot{i}", f"Slot {i + 1}")
            ed = SlotEditor(i)
            ed.changed.connect(self._mark_dirty)
            self.slot_editors.append(ed)
            self._register_page(ed)

        # Inventory (four subclasses + packs + important items) -------------
        self._add_section("INVENTORY")
        self.inventory_pages = []
        for label, page in make_inventory_pages():
            nav("inv-" + label.lower(), label)
            self.inventory_pages.append(page)
            self._register_page(page)
        nav("packs", "Card Packs")
        self.packs_tab = PacksTab()
        self._register_page(self.packs_tab)
        nav("keyitems", "Important Items")
        self.key_items_tab = KeyItemsTab()
        self._register_page(self.key_items_tab)

        # Partners / collection ------------------------------------------------
        self._add_section("PARTNERS & COLLECTION")
        nav("digimon", "Digimon")
        self.digimon_tab = DigimonTab()
        self._register_page(self.digimon_tab)
        nav("digivolution", "Digivolution")
        self.digivolution_tab = DigivolutionTab()
        self._register_page(self.digivolution_tab)
        nav("cards", "Cards")
        self.cards_tab = CardsTab()
        self._register_page(self.cards_tab)

        # Tools / developer ----------------------------------------------------
        self._add_section("TOOLS")
        nav("fieldlocator", "Field Locator")
        self.field_locator = FieldLocator()
        self._register_page(self.field_locator)
        nav("hex", "Hex Editor")
        self.hex_page = HexEditorPage()
        self._register_page(self.hex_page)

        # Appearance ------------------------------------------------------------
        self._add_section("APPEARANCE")
        nav("themes", "Themes")
        self.themes_tab = ThemesTab()
        self._register_page(self.themes_tab)
        self.themes_tab.refresh()

        self.sidebar_layout.addStretch(1)
        # default to first page
        self.stack.setCurrentIndex(0)
        self._nav_buttons[0].setChecked(True)

    def _register_page(self, widget: QWidget) -> None:
        self.stack.addWidget(widget)

    def _select_page(self, key: str) -> None:
        if key in self._keys:
            idx = self._keys.index(key)
            self.stack.setCurrentIndex(idx)
            self._nav_buttons[idx].setChecked(True)

    # -- menu ----------------------------------------------------------------
    def _build_menu(self) -> None:
        m = self.menuBar().addMenu("&File")
        act_open = QAction("&Open card…", self)
        act_open.setShortcut(QKeySequence.Open)
        act_open.triggered.connect(self.open_file)
        m.addAction(act_open)
        self.act_save = QAction("&Save", self)
        self.act_save.setShortcut(QKeySequence.Save)
        self.act_save.triggered.connect(self.save_file)
        self.act_save.setEnabled(False)
        m.addAction(self.act_save)
        self.act_save_as = QAction("Save &as…", self)
        self.act_save_as.setShortcut(QKeySequence.SaveAs)
        self.act_save_as.triggered.connect(self.save_file_as)
        self.act_save_as.setEnabled(False)
        m.addAction(self.act_save_as)
        m.addSeparator()
        act_quit = QAction("&Quit", self)
        act_quit.setShortcut(QKeySequence.Quit)
        act_quit.triggered.connect(self.close)
        m.addAction(act_quit)

    # -- state --------------------------------------------------------------
    def _mark_dirty(self) -> None:
        self._dirty = True
        self._refresh_titles()

    def _refresh_titles(self) -> None:
        if self._save is None:
            return
        mark = " •" if self._dirty else ""
        name = self._path.name if self._path else "card"
        self.setWindowTitle(f"{APP_NAME}, {name}{mark}")
        for i, ed in enumerate(self.slot_editors):
            slot = self._save.slot(i)
            label = f"Slot {i + 1}"
            if slot.is_empty:
                label += "  (empty)"
            self._nav_buttons[i].setText(label)

    def _sync_enabled(self) -> None:
        """Enable save-dependent pages only when a card is loaded.

        The Themes page (appearance) stays usable even with no card open, so
        instead of disabling the whole stack we disable each save-dependent
        page individually.
        """
        enabled = self._save is not None
        self.stack.setEnabled(True)
        for page in self._save_pages():
            page.setEnabled(enabled)
        for key, btn in zip(self._keys, self._nav_buttons):
            btn.setEnabled(enabled or key == "themes")
        self.save_btn.setEnabled(enabled)
        if hasattr(self, "act_save"):
            self.act_save.setEnabled(enabled)
            self.act_save_as.setEnabled(enabled)
        # keep dev toggle always available
        self.dev_toggle.setEnabled(True)

    def _save_pages(self) -> list[QWidget]:
        """Every page that edits a loaded save (Themes excluded)."""
        pages: list[QWidget] = []
        pages.extend(self.slot_editors)
        pages.extend(self.inventory_pages)
        pages.append(self.packs_tab)
        pages.append(self.digimon_tab)
        pages.append(self.digivolution_tab)
        pages.append(self.cards_tab)
        pages.append(self.key_items_tab)
        pages.append(self.field_locator)
        pages.append(self.hex_page)
        return pages

    def _on_dev_toggle(self, checked: bool) -> None:
        self.hex_page.set_dev_mode(checked)

    def _bind_all(self, save: DMW3Save) -> None:
        """Bind every editor page to a save payload."""
        for ed in self.slot_editors:
            ed.bind(save)
        for page in self.inventory_pages:
            page.bind(save)
        self.packs_tab.bind(save)
        self.digimon_tab.bind(save)
        self.digivolution_tab.bind(save)
        self.cards_tab.bind(save)
        self.key_items_tab.bind(save)
        self.hex_page.bind(save)
        self.hex_page.set_dev_mode(self.dev_toggle.isChecked())

    def _on_region_changed(self, *_args) -> None:
        if self._save is None:
            return
        region = self.region_combo.currentData()
        try:
            if region == "auto":
                self._save.clear_region_override()
            else:
                self._save.force_region(region)
        except SaveError as exc:
            QMessageBox.critical(self, "Region", str(exc))
            return
        # re-interpret pages under the chosen region
        self._bind_all(self._save)
        self._sync_enabled()
        self.subtitle.setText(
            f"{self._loc.name}  ·  region {self._save.region_guess} "
            f"{'' if region == 'auto' else '(manual)'}  ·  checksum "
            f"{'valid' if self._save.checksum_valid else 'INVALID'}"
        )
        self._refresh_titles()

    # -- actions ------------------------------------------------------------
    def open_file(self) -> None:
        path, _ = QFileDialog.getOpenFileName(
            self, "Open PS1 memory card", "", FILTER
        )
        if not path:
            return
        try:
            card = mc.MemoryCard.load(path)
            loc = card.find_dmw3_save()
            if loc is None:
                raise SaveError(
                    "No Digimon World 3 save found on this card. Expected an "
                    "entry named BASLUS-01436…, BESLES-03936… or BISLPS-03050…"
                )
            save = DMW3Save(card.extract_payload(loc))
        except Exception as exc:  # noqa: BLE001 - surfaced to the user
            QMessageBox.critical(self, "Could not open card", str(exc))
            return

        self._card, self._loc, self._save = card, loc, save
        self._path = pathlib.Path(path)
        self._dirty = False

        # Sync the ROM-region selector to the card's detected version. A prior
        # manual override is reset on every new card (blocking the handler so
        # it does not re-bind mid-load).
        self.region_combo.blockSignals(True)
        idx = self.region_combo.findData(save.region_guess if save.region_guess in ("USA", "EUR") else "auto")
        self.region_combo.setCurrentIndex(max(idx, 0))
        self.region_combo.blockSignals(False)

        self._bind_all(save)
        self._sync_enabled()
        self.subtitle.setText(
            f"{loc.name}  ·  region {save.region_guess}  ·  "
            f"checksum {'valid' if save.checksum_valid else 'INVALID'}"
        )
        self._refresh_titles()
        self.statusBar().showMessage(f"Loaded {self._path.name}", 6000)
        # show slot 1 by default
        self.stack.setCurrentIndex(0)
        self._nav_buttons[0].setChecked(True)

    def _write_to(self, path: pathlib.Path) -> None:
        assert self._card and self._save and self._loc
        if path.exists():
            backup = path.with_suffix(path.suffix + ".bak")
            if not backup.exists():
                shutil.copy2(path, backup)
        self._card.reinsert_payload(self._save.to_bytes(), self._loc)
        self._card.save(str(path))
        self._path = path
        self._dirty = False
        self._refresh_titles()
        self.statusBar().showMessage(
            f"Saved {path.name} (original backed up as {path.name}.bak)", 8000
        )

    def save_file(self) -> None:
        if self._path is None:
            self.save_file_as()
            return
        try:
            self._write_to(self._path)
        except Exception as exc:  # noqa: BLE001
            QMessageBox.critical(self, "Could not save", str(exc))

    def save_file_as(self) -> None:
        path, _ = QFileDialog.getSaveFileName(
            self, "Save memory card as", str(self._path or ""), FILTER
        )
        if not path:
            return
        try:
            self._write_to(pathlib.Path(path))
        except Exception as exc:  # noqa: BLE001
            QMessageBox.critical(self, "Could not save", str(exc))

    def closeEvent(self, event) -> None:  # noqa: N802 - Qt signature
        if not self._dirty:
            event.accept()
            return
        choice = QMessageBox.question(
            self,
            "Unsaved changes",
            "You have unsaved edits. Save before closing?",
            QMessageBox.Save | QMessageBox.Discard | QMessageBox.Cancel,
        )
        if choice == QMessageBox.Save:
            self.save_file()
            event.accept()
        elif choice == QMessageBox.Discard:
            event.accept()
        else:
            event.ignore()


def main() -> int:
    app = QApplication(sys.argv)
    app.setApplicationName(APP_NAME)
    theme.apply(app, saved_theme())
    win = MainWindow()
    win.show()
    return app.exec()
