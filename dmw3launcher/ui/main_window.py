"""The launcher shell: header and tabs. The window's own close control is the only way out."""
from __future__ import annotations

import pathlib

from PySide6.QtCore import Qt, QSize
from PySide6.QtGui import QColor, QIcon, QPainter, QPixmap
from PySide6.QtWidgets import (QFrame, QHBoxLayout, QLabel, QMainWindow, QPushButton,
                               QTabWidget, QVBoxLayout, QWidget)

from .. import builds, paths
from . import theme
from .decomp_tab import DecompTab
from .memcard_tab import MemcardTab
from .mods_tab import ModsTab
from .play_tab import PlayTab
from .settings_tab import SettingsTab

VERSION = "0.1.0"
# The logo is drawn at the header bar's own height (its art is 727x320, a wordmark, not an icon).
LOGO_ART_H = 64

# One source of truth for the tab bar: the order the tabs appear in, and the name each one carries
# as a tooltip and as its text fallback when its word-image is missing.
TAB_ORDER = ("play", "memcard", "mods", "decomp", "settings")
TAB_LABELS = {"play": "Play", "memcard": "Memory Card", "mods": "Mods",
              "decomp": "Decompilation", "settings": "Settings"}


class MainWindow(QMainWindow):
    def __init__(self, cfg: dict) -> None:
        super().__init__()
        self.cfg = cfg
        self._bg: QPixmap | None = theme.background()

        self.setWindowTitle(paths.APP_NAME)
        self.setWindowIcon(theme.app_icon())
        self.resize(1200, 880)
        # Five tab word-images need ~990px of bar (measured), so the window cannot be narrowed to
        # 900 any more without the tab bar scrolling its own labels out of view. The height is what
        # the Decompilation page needs before its splitter starts squeezing its panes: with the
        # page's real margins the splitter holds its floor from 880px (swept 760..990), so the
        # minimum matches the default height instead of squeezing the page shorter than it is.
        self.setMinimumSize(1060, 880)
        self.setStyleSheet(theme.stylesheet())

        root = QWidget()
        self.setCentralWidget(root)
        outer = QVBoxLayout(root)
        outer.setContentsMargins(0, 0, 0, 0)
        outer.setSpacing(0)

        # ---- header -------------------------------------------------------
        # No explicit fill: the header tracks the live palette, so a theme swap recolours it.
        # The header is filled with the PAGE colour rather than a panel tone, so the logo sits
        # seamlessly on it and the header reads as part of the page instead of a separate band.
        self.header = theme.ArtFrame(theme.header_bar(), fill=theme.BG)
        self.header.setObjectName("header")
        hl = QHBoxLayout(self.header)
        hl.setContentsMargins(theme.PAGE_MARGIN, 12, theme.PAGE_MARGIN, 12)
        hl.setSpacing(theme.CARD_GAP)

        # The logo is the header's identity, so no text sits next to it. The text titles only exist
        # for the case where no logo was found at all, so the header is never blank.
        logo = theme.logo()
        if logo is not None:
            lbl = QLabel()
            lbl.setPixmap(logo.scaledToHeight(LOGO_ART_H,
                                              Qt.TransformationMode.SmoothTransformation))
            hl.addWidget(lbl, 0, Qt.AlignmentFlag.AlignVCenter)
        else:
            titles = QVBoxLayout()
            titles.setSpacing(0)
            t = QLabel("Digimon World 3")
            t.setObjectName("h1")
            s = QLabel("Native launcher")
            s.setObjectName("dim")
            titles.addWidget(t)
            titles.addWidget(s)
            hl.addLayout(titles)
        hl.addStretch(1)

        self.btn_folder = QPushButton("Open Builds folder")
        self.btn_folder.clicked.connect(self._open_install)
        hl.addWidget(self.btn_folder)
        # No redundant in-app close button: the window already carries the standard close control,
        # so a second one only duplicated it. Closing is the title bar's job (Alt+F4 / the X).
        outer.addWidget(self.header)

        # ---- tabs ---------------------------------------------------------
        self.tabs = QTabWidget()
        self.tabs.setDocumentMode(True)
        self.play = PlayTab(cfg)
        self.memcard = MemcardTab(cfg)
        self.mods = ModsTab(cfg)
        self.decomp = DecompTab(cfg)
        self.settings = SettingsTab(cfg)

        # The tabs carry the user's own art: the tab NAME as a word-image (320x80) instead of a
        # text label. Re-applied on every restyle, because the auto-tint follows the palette.
        # TAB_ORDER/TAB_LABELS are module constants so the verifier and this list cannot drift.
        self._tab_widgets = {"play": self.play, "memcard": self.memcard, "mods": self.mods,
                             "decomp": self.decomp, "settings": self.settings}
        self._tab_keys = tuple((key, TAB_LABELS[key], self._tab_widgets[key])
                               for key in TAB_ORDER)
        for _key, _label, _widget in self._tab_keys:
            self.tabs.addTab(_widget, "")
        self._apply_tab_art()

        holder = QWidget()
        hl2 = QVBoxLayout(holder)
        # The tab bar sits on the same left gutter as page content, so tab labels line up with
        # the text below them instead of being indented by an arbitrary amount.
        hl2.setContentsMargins(theme.PAGE_MARGIN, 6, theme.PAGE_MARGIN, theme.CARD_GAP)
        hl2.addWidget(self.tabs)
        outer.addWidget(holder, 1)

        # keep the Memory Card tab in sync when an install is located elsewhere
        self.tabs.currentChanged.connect(self._tab_changed)

        # the status bar carries information, not the app name twice: which regional builds are
        # ready beside the launcher (the Play tab makes them from the player's own disc).
        _ready = [s.label for s in builds.BUILDS if builds.build_status(s.region)[0]]
        self.statusBar().showMessage(
            f"{paths.APP_NAME} {VERSION} — builds: "
            + (", ".join(_ready) if _ready
               else "none yet — build one from your own disc on the Play tab"))

        # The embedded save editor themes the whole application, so the launcher's own chrome
        # has to follow it. Registering here means a theme picked INSIDE the editor recolours
        # this window too, instead of leaving two palettes on screen at once.
        theme.on_change(self._restyle)
        self._restyle()      # the Memory Card tab may already have activated the editor theme

    # ------------------------------------------------------------------ theme
    def _restyle(self) -> None:
        """Re-apply everything that bakes a palette colour into a stylesheet or a paint.

        When the embedded save editor drives the app-wide theme, the launcher's own sheet goes on
        its own panels instead of on the window. The window is an ancestor of the embedded editor
        and Qt merges rule sets, matching by specificity: rules the editor's sheet does not fully
        specify (its QGroupBox sets no `padding`, its QWidget rule paints no background on purpose)
        would otherwise leak in and shift the editor's own widgets. The tab bar is the one piece
        that cannot be styled per-page, so it gets a sheet whose selectors cannot match anything
        inside the editor.
        """
        css = theme.stylesheet()
        if theme.editor_theme_active():
            self.setStyleSheet("")
            self.tabs.setStyleSheet(theme.tabbar_stylesheet())
            for w in (self.header, self.play, self.mods, self.decomp, self.settings):
                w.setStyleSheet(css)
            self.memcard.style_chrome(css)
        else:
            self.setStyleSheet(css)
        self.header.set_fill(theme.BG)      # page colour, so header and page never seam
        self.setWindowIcon(theme.app_icon())
        self._apply_tab_art()
        self.update()

    def _apply_tab_art(self) -> None:
        """Draw the user's tab art in the tab bar, falling back to text when a slot is empty.

        These are word-images rather than square glyphs, so one art height governs the whole bar and
        each icon keeps its own aspect. A tooltip carries the name now that no text is drawn, and
        the selected tab keeps the accent underline under the word rather than around it.
        """
        sized = False
        art = theme.tab_images(theme.TAB_ART_H)      # one load for the whole row
        for index, (key, label, _widget) in enumerate(self._tab_keys):
            pm = art.get(key)
            if pm is None:
                self.tabs.setTabIcon(index, QIcon())
                self.tabs.setTabText(index, label)
                self.tabs.setTabToolTip(index, "")
                continue
            self.tabs.setTabIcon(index, QIcon(pm))
            self.tabs.setTabText(index, "")
            self.tabs.setTabToolTip(index, label)
            if not sized:
                self.tabs.setIconSize(QSize(pm.width(), pm.height()))
                sized = True

    # ------------------------------------------------------------------ events
    def _tab_changed(self, index: int) -> None:
        w = self.tabs.widget(index)
        if hasattr(w, "refresh") and w is not self.play:
            try:
                w.refresh()
            except Exception:
                pass

    def _open_install(self) -> None:
        """Reveal where the native builds live (the builder drops them there)."""
        from .. import runtime
        d = builds.builds_dir()
        d.mkdir(parents=True, exist_ok=True)
        runtime.open_path(d)

    def paintEvent(self, event) -> None:  # noqa: N802 (Qt naming)
        super().paintEvent(event)
        p = QPainter(self)
        target = self.rect()
        if self._bg is None:
            # No background art, so the window is the live theme's own primary colour: it follows
            # whichever theme is active instead of needing a picture. (theme.BG is rewritten by
            # use_editor_theme, so this tracks the embedded editor's choice too.)
            p.fillRect(target, QColor(theme.BG))
            p.end()
            return
        scaled = self._bg.scaled(target.size(), Qt.AspectRatioMode.KeepAspectRatioByExpanding,
                                 Qt.TransformationMode.SmoothTransformation)
        x = (target.width() - scaled.width()) // 2
        y = (target.height() - scaled.height()) // 2
        p.setOpacity(0.28)
        p.drawPixmap(int(x), int(y), scaled)
        p.end()
