"""The Launcher Settings tab.

Writes a regional build's own `settings.toml` (the file the runtime reads and writes next to its
exe) through a comment-preserving writer, so the documented comments and any key the launcher does
not know about survive untouched. Controller bindings live beside it in `input.ini` and are never
rewritten here, only revealed.

Each build keeps its own settings, so the tab works on one build at a time; the region picker leads
with USA, the same order the Play tab uses.
"""
from __future__ import annotations

import pathlib
from typing import Iterable

from PySide6.QtCore import QRect, QSettings, Qt
from PySide6.QtGui import QColor, QIcon, QPainter, QPen, QPixmap
from PySide6.QtWidgets import (QApplication, QCheckBox, QComboBox, QFormLayout, QGridLayout,
                               QGroupBox, QHBoxLayout, QLabel, QMessageBox, QPushButton,
                               QScrollArea, QSizePolicy, QSpinBox, QVBoxLayout, QWidget)

from .. import builds, paths, runtime
from .. import settings as st
from . import theme

# Layout rules for this page. Every group's form shares ONE label column, so the fields line up
# page-wide; letting each QFormLayout size its own label column is what made the groups disagree
# (Video's fields started at x=250, Loading's at x=159, Audio's toggles at x=29).
LABEL_COL_W = 190   # the shared label column; "Texture filter" (182px) sets the floor
FIELD_W = 160       # one width for every dropdown, so the field column reads as a column
SPIN_W = 90
CHECK_COLS = 2      # toggles sit in a grid: five of them are three rows, not five
# Heights are left to the widgets: pinning a fixed height was rewritten by the layout (Qt resizes a
# fixed-size widget when it squeezes the row), so the constant asserted something it did not hold.


def palette_icon(pal: dict, width: int = 30, height: int = 14) -> QIcon:
    """A theme's colours as a tiny stripe, for a dropdown entry: the choice is visible while picking."""
    pm = QPixmap(width, height)
    pm.fill(Qt.GlobalColor.transparent)
    p = QPainter(pm)
    p.setRenderHint(QPainter.RenderHint.Antialiasing, True)
    tokens = [pal.get(t) for t in ("BG", "SURFACE_2", "ACCENT")]
    stripe = width / max(1, len(tokens))
    for i, colour in enumerate(tokens):
        if not colour:
            continue
        p.setPen(Qt.PenStyle.NoPen)
        p.setBrush(QColor(colour))
        p.drawRect(int(i * stripe), 0, int(stripe) + 1, height)
    p.setBrush(Qt.BrushStyle.NoBrush)
    p.setPen(QPen(QColor(pal.get("BORDER_STRONG") or "#000000"), 1))
    p.drawRoundedRect(0, 0, width - 1, height - 1, 2, 2)
    p.end()
    return QIcon(pm)


class _PaletteStrip(QWidget):
    """The selected theme's whole palette as chips, before and after it is applied."""

    TOKENS = ("BG", "SURFACE", "SURFACE_2", "BORDER_STRONG", "ACCENT", "OK", "WARN", "DANGER")
    CHIP_W, CHIP_H, GAP = 26, 18, 6

    def __init__(self, parent=None) -> None:
        super().__init__(parent)
        self._pal: dict = {}
        self.setFixedHeight(self.CHIP_H)
        self.setMinimumWidth(len(self.TOKENS) * (self.CHIP_W + self.GAP))
        self.setSizePolicy(QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Fixed)

    def set_palette(self, pal: dict | None) -> None:
        self._pal = dict(pal or {})
        shown = [f"{t} {self._pal[t]}" for t in self.TOKENS if self._pal.get(t)]
        self.setToolTip("  ·  ".join(shown) or "no palette")
        self.update()

    def paintEvent(self, event) -> None:  # noqa: N802 (Qt naming)
        p = QPainter(self)
        p.setRenderHint(QPainter.RenderHint.Antialiasing, True)
        for index, token in enumerate(self.TOKENS):
            x = index * (self.CHIP_W + self.GAP)
            rect = QRect(x, 0, self.CHIP_W, self.CHIP_H)
            colour = self._pal.get(token)
            if colour:
                p.setPen(Qt.PenStyle.NoPen)
                p.setBrush(QColor(colour))
                p.drawRoundedRect(rect, 4, 4)
            # A chip the same colour as the panel it sits on would vanish, so every one is outlined.
            p.setBrush(Qt.BrushStyle.NoBrush)
            p.setPen(QPen(QColor(theme.BORDER), 1))
            p.drawRoundedRect(rect.adjusted(0, 0, -1, -1), 4, 4)
        p.end()


class SettingsTab(QWidget):
    def __init__(self, cfg: dict) -> None:
        super().__init__()
        self.cfg = cfg
        self.region: str = builds.default_region()
        self._widgets: dict[tuple[str, str], QWidget] = {}
        self._mapped: set[tuple[str, str]] = set()   # combos whose value lives in itemData
        self._ed_theme = None            # the save editor's theme module, when it is importable
        self._ed_theme_error = ""

        outer = QVBoxLayout(self)
        outer.setContentsMargins(theme.PAGE_MARGIN, theme.CARD_GAP,
                                 theme.PAGE_MARGIN, theme.CARD_GAP)
        outer.setSpacing(theme.CARD_GAP)

        self.head = QLabel()
        self.head.setWordWrap(True)
        self.head.setObjectName("hint")
        outer.addWidget(self.head)

        pick = QHBoxLayout()
        pick.setSpacing(theme.ROW_GAP)
        pick.addWidget(QLabel("Build"))
        self.combo_region = QComboBox()
        for b in builds.BUILDS:
            self.combo_region.addItem(b.label, b.region)
        self.combo_region.setFixedWidth(FIELD_W)
        self.combo_region.currentIndexChanged.connect(self._pick_region)
        pick.addWidget(self.combo_region)
        pick.addStretch(1)
        outer.addLayout(pick)

        scroll = QScrollArea()
        scroll.setWidgetResizable(True)
        inner = QWidget()
        scroll.setWidget(inner)
        v = QVBoxLayout(inner)
        # the page already carries PAGE_MARGIN; only leave room for the scrollbar
        v.setContentsMargins(0, 0, theme.ROW_GAP, 0)
        v.setSpacing(theme.CARD_GAP)

        # Appearance sits here rather than in the Memory Card tab: it is the launcher's own control
        # for the one app-wide theme, the save editor's 5 palettes. It applies the moment it changes
        # (no Apply button) and is deliberately NOT in self._widgets, because it is not a key in the
        # runtime's settings.toml and must not be written there.
        self.g_look, look_form = self._group("Appearance")
        self.look_combo = theme.ComboBox()
        self.look_combo.setFixedWidth(FIELD_W)
        look_form.addRow(self._label("Theme"), self.look_combo)
        self.look_swatches = _PaletteStrip()
        look_form.addRow(self._label("Palette"), self.look_swatches)
        self.look_tag = QLabel("")
        self.look_tag.setObjectName("dim")
        look_form.addRow(self.look_tag)
        self.look_hint = QLabel("Sets the look of the whole launcher and of the save editor at "
                                "once, and is remembered for the next launch.")
        self.look_hint.setObjectName("dim")
        self.look_hint.setWordWrap(True)
        look_form.addRow(self.look_hint)
        v.addWidget(self.g_look)
        self._load_themes()

        self.g_video, video_form = self._group("Video")
        for key, label, opts in st.VIDEO_CHOICES:
            self._combo("video", key, label, opts, video_form)
        self._value_combo("video", "fullscreen", "Fullscreen", st.FULLSCREEN_CHOICES, video_form)
        self._value_combo("video", "window_width", "Window size", st.WINDOW_SIZE_CHOICES,
                          video_form)
        for key, label, lo, hi in st.VIDEO_INTS:
            self._spin("video", key, label, lo, hi, video_form)
        self._checks("video", st.VIDEO_BOOLS, video_form)
        v.addWidget(self.g_video)

        self.g_audio, audio_form = self._group("Audio")
        self._checks("audio", st.AUDIO_BOOLS, audio_form)
        v.addWidget(self.g_audio)

        self.g_load, load_form = self._group("Loading")
        for key, label, opts in st.LOAD_CHOICES:
            self._combo("localization", key, label, opts, load_form)
        self._combo("localization", "load_sectors_per_frame", "Sectors per frame", st.LOAD_STEPS,
                    load_form)
        self._checks("video", st.LOADING_BOOLS, load_form)   # stored in [video], grouped here
        load_hint = QLabel("Instant serves the data the moment the game asks for it; 1x imitates "
                           "the console's drive (4 to 9 seconds per zone). Sectors per frame is "
                           "the safety knob: 96 is the tested value, and it is what you lower if "
                           "a zone wedges.")
        load_hint.setObjectName("dim")
        load_hint.setWordWrap(True)
        load_form.addRow(load_hint)
        v.addWidget(self.g_load)

        self.g_ctrl, ctrl_form = self._group("Controller")
        note = QLabel("Bindings live in the build's own input.ini and keybinds.ini, the "
                      "launcher leaves those files alone. These are the runtime's own "
                      "controller settings.")
        note.setObjectName("dim")
        note.setWordWrap(True)
        ctrl_form.addRow(note)
        self.lbl_ctrl = QLabel()
        ctrl_form.addRow(self.lbl_ctrl)
        self.btn_input = QPushButton("Open input.ini folder")
        self.btn_input.clicked.connect(self._open_inputs)
        # addRow(widget) stretches the widget across the whole row; a stretch beside it keeps the
        # button at its own width instead of a 1068px-wide slab.
        btn_row = QHBoxLayout()
        btn_row.addWidget(self.btn_input)
        btn_row.addStretch(1)
        ctrl_form.addRow(btn_row)
        v.addWidget(self.g_ctrl)

        v.addStretch(1)
        outer.addWidget(scroll, 1)

        bar = QHBoxLayout()
        bar.setSpacing(theme.ROW_GAP)
        self.lbl_note = QLabel()
        self.lbl_note.setObjectName("hint")
        self.btn_reload = QPushButton("Reload from disk")
        self.btn_reload.clicked.connect(self.reload)
        self.btn_raw = QPushButton("Open settings.toml")
        self.btn_raw.clicked.connect(self._open_raw)
        self.btn_apply = QPushButton("Apply")
        self.btn_apply.setObjectName("primary")
        self.btn_apply.clicked.connect(self.apply)
        # note on the left, actions on the right, primary last: one reading order per page
        bar.addWidget(self.lbl_note, 1)
        bar.addWidget(self.btn_reload)
        bar.addWidget(self.btn_raw)
        bar.addWidget(self.btn_apply)
        outer.addLayout(bar)

        index = self.combo_region.findData(self.region)
        if index >= 0:
            self.combo_region.blockSignals(True)
            self.combo_region.setCurrentIndex(index)
            self.combo_region.blockSignals(False)
        self.reload()

    # ------------------------------------------------------------------ appearance
    def _editor_theme(self):
        """The save editor's theme module, or None when the editor cannot be imported.

        Lazy and guarded on purpose: the editor is an optional dependency of this page (it needs
        numpy), and a missing one must show a line of explanation instead of taking the tab down.
        """
        if self._ed_theme is not None:
            return self._ed_theme
        if self._ed_theme_error:
            return None
        try:
            from dmw3editor.ui import theme as ed_theme
        except Exception as exc:  # noqa: BLE001
            self._ed_theme_error = (f"The save editor's themes are unavailable "
                                    f"({type(exc).__name__}: {exc}).")
            return None
        self._ed_theme = ed_theme
        return ed_theme

    def _saved_theme(self) -> str:
        try:
            from dmw3editor.ui.themes_tab import saved_theme
            return str(saved_theme())
        except Exception:  # noqa: BLE001
            return ""

    def _load_themes(self) -> None:
        """Fill the dropdown from the editor's palettes and preselect the saved choice."""
        ed_theme = self._editor_theme()
        if ed_theme is None:
            self.look_combo.setEnabled(False)
            self.look_hint.setText(self._ed_theme_error)
            return
        for key, pal in ed_theme.PALETTES.items():
            # The entry carries its own colours, so the list shows what it is offering.
            self.look_combo.addItem(palette_icon(pal), ed_theme.theme_label(key), key)
        index = self.look_combo.findData(self._saved_theme())
        if index >= 0:
            self.look_combo.blockSignals(True)
            self.look_combo.setCurrentIndex(index)
            self.look_combo.blockSignals(False)
        self._show_palette()
        self.look_combo.currentIndexChanged.connect(self._pick_theme)

    def _show_palette(self) -> None:
        """Point the swatches and the one-line description at the palette on screen."""
        ed_theme = self._editor_theme()
        key = self.look_combo.currentData()
        pal = (ed_theme.PALETTES.get(key) or {}) if ed_theme else {}
        self.look_swatches.set_palette(pal)
        self.look_tag.setText(str(pal.get("tag", "")))

    def _pick_theme(self) -> None:
        """Apply the chosen theme app-wide and remember it, the way the editor's own picker does."""
        ed_theme = self._editor_theme()
        key = self.look_combo.currentData()
        if ed_theme is None or not key:
            return
        self._show_palette()
        QSettings("DMW3SaveEditor", "DMW3SaveEditor").setValue("theme", key)
        app = QApplication.instance()
        if app is not None:
            ed_theme.apply(app, key)     # the Memory Card tab's hook restyles the launcher with it
        pal = ed_theme.PALETTES.get(key)
        if pal:
            # Belt and braces: that hook belongs to the Memory Card tab, and this control has to
            # work even when the embedded editor failed to load.
            theme.set_editor_active(True)
            theme.use_editor_theme(pal)

    # ------------------------------------------------------------------ builders
    def _group(self, title: str) -> tuple[QGroupBox, QFormLayout]:
        g = QGroupBox(title)
        f = QFormLayout(g)
        f.setContentsMargins(theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD)
        f.setVerticalSpacing(theme.ROW_GAP)
        f.setHorizontalSpacing(theme.CARD_GAP)
        f.setLabelAlignment(Qt.AlignmentFlag.AlignLeft | Qt.AlignmentFlag.AlignVCenter)
        # Fields keep their natural width instead of stretching to the whole card, which is what
        # made every dropdown a metre wide.
        f.setFieldGrowthPolicy(QFormLayout.FieldGrowthPolicy.FieldsStayAtSizeHint)
        f.setFormAlignment(Qt.AlignmentFlag.AlignLeft | Qt.AlignmentFlag.AlignTop)
        return g, f

    def _label(self, text: str) -> QLabel:
        """A form label holding the shared column width, so fields line up across groups."""
        w = QLabel(text)
        w.setMinimumWidth(LABEL_COL_W)
        return w

    def _checks(self, sect: str, pairs: Iterable[tuple[str, str]], form: QFormLayout) -> None:
        """Toggles as a grid in ONE form row: in the field column, compact vertically."""
        host = QWidget()
        grid = QGridLayout(host)
        grid.setContentsMargins(0, 0, 0, 0)
        grid.setHorizontalSpacing(theme.CARD_GAP)
        grid.setVerticalSpacing(theme.ROW_GAP)
        for i, (key, label) in enumerate(pairs):
            cb = QCheckBox(label)
            grid.addWidget(cb, i // CHECK_COLS, i % CHECK_COLS)
            self._widgets[(sect, key)] = cb
        # An empty label of the shared width, so a group with no fields of its own still starts its
        # toggles in the same column as every other group's fields.
        form.addRow(self._label(""), host)

    def _combo(self, sect: str, key: str, label: str, opts: tuple[str, ...],
               form: QFormLayout) -> None:
        w = theme.ComboBox()      # caret painted by us: Qt's ::down-arrow double-drew it
        w.addItems(list(opts))
        w.setFixedWidth(FIELD_W)
        form.addRow(self._label(label), w)
        self._widgets[(sect, key)] = w

    def _value_combo(self, sect: str, key: str, label: str,
                     presets: tuple[tuple[str, int], ...], form: QFormLayout) -> None:
        """A dropdown that shows a readable label but stores the raw value the runtime wants."""
        w = theme.ComboBox()      # caret painted by us: Qt's ::down-arrow double-drew it
        for text, value in presets:
            w.addItem(text, value)
        w.preset_values = [value for _, value in presets]
        w.setFixedWidth(FIELD_W)
        form.addRow(self._label(label), w)
        self._widgets[(sect, key)] = w
        self._mapped.add((sect, key))

    def _spin(self, sect: str, key: str, label: str, lo: int, hi: int,
              form: QFormLayout) -> None:
        w = QSpinBox()
        w.setRange(lo, hi)
        w.setFixedWidth(SPIN_W)
        form.addRow(self._label(label), w)
        self._widgets[(sect, key)] = w

    # ------------------------------------------------------------------ data
    def _pick_region(self) -> None:
        self.region = self.combo_region.currentData() or builds.default_region()
        self.reload()

    def reload(self) -> None:
        p = builds.settings_toml(self.region)
        label = builds.spec(self.region).label
        if not builds.build_status(self.region)[0]:
            self.head.setText(f"The {label} build is not on disk yet, build it on the Play tab "
                              f"first, and its settings.toml ({p}) appears here.")
            self.setEnabled(True)
            self.btn_apply.setEnabled(False)
            self.lbl_ctrl.setText("not set")
            return
        self.btn_apply.setEnabled(True)
        self.head.setText(f"These are the {label} build's own settings ({p}).")
        if not p.is_file():
            self.head.setText(f"The {label} build has no settings.toml yet ({p}). Start it once "
                              f"and the runtime writes one; the defaults below are what it will use.")
        if not st.available():
            # Refuse to touch the file rather than rewrite it without comment support.
            self.head.setText(
                f"tomlkit is not installed, so the launcher will not rewrite\n{p}\n"
                f"(it always preserves that file's comments). Install it with\n\n"
                f"    pip install tomlkit\n")
            self.btn_apply.setEnabled(False)
            return
        snap = st.snapshot(p) if p.is_file() else {}

        for (sect, key), w in self._widgets.items():
            val = snap.get(sect, {}).get(key)
            if val is None:
                continue
            if isinstance(w, QCheckBox):
                w.setChecked(bool(val))
            elif isinstance(w, QComboBox):
                if (sect, key) in self._mapped:
                    # drop a "(custom)" entry from an earlier reload before deciding again
                    for j in range(w.count() - 1, -1, -1):
                        if w.itemData(j) not in w.preset_values:
                            w.removeItem(j)
                    try:
                        want = int(val)     # tomlkit hands back an Integer, which Qt's findData
                    except (TypeError, ValueError):    # will not match against the stored ints
                        want = val
                    i = w.findData(want)
                    if i < 0:                       # a size outside the presets: keep it visible
                        w.addItem(f"{val} (custom)", val)
                        i = w.count() - 1
                else:
                    i = w.findText(str(val))
                    if i < 0:
                        w.addItem(str(val))
                        i = w.count() - 1
                w.setCurrentIndex(i)
            elif isinstance(w, QSpinBox):
                try:
                    w.setValue(int(val))
                except (TypeError, ValueError):
                    pass

        ctrl = runtime.controller_summary(p)
        self.lbl_ctrl.setText("\n".join(f"{k} = {v}" for k, v in ctrl.items()) or "not set")
        self.lbl_note.setText("unknown keys and comments are preserved")

    def apply(self) -> None:
        p = builds.settings_toml(self.region)
        updates: dict[tuple[str, str], object] = {}
        for (sect, key), w in self._widgets.items():
            if isinstance(w, QCheckBox):
                updates[(sect, key)] = bool(w.isChecked())
            elif isinstance(w, QComboBox):
                # a size combo stores the width, not the label the player reads
                updates[(sect, key)] = (int(w.currentData()) if (sect, key) in self._mapped
                                        else w.currentText())
            elif isinstance(w, QSpinBox):
                updates[(sect, key)] = int(w.value())
        try:
            st.set_values(p, updates)
        except Exception as exc:  # noqa: BLE001
            QMessageBox.critical(self, "Could not write settings",
                                 f"{exc}\n\nIs tomlkit installed? (pip install tomlkit)")
            return
        self.lbl_note.setText("applied")
        QMessageBox.information(self, "Settings", "Written to settings.toml. Start the game "
                                                  "to use them.")

    def _open_raw(self) -> None:
        p = builds.settings_toml(self.region)
        if p.is_file():
            runtime.open_path(p)

    def _open_inputs(self) -> None:
        d = builds.build_dir(self.region)
        if d.is_dir():
            runtime.open_path(d)
