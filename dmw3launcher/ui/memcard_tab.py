"""The Memory Card tab.

Wires the player's saves straight into the existing save editor, in-process, so there is one
window and no file dialog: the tab finds the saves the runtime is using and opens them.

The editor's WHOLE main window is embedded (a QMainWindow parented as a child widget), so its
menu bar, status bar and pages all come along and the tab is the standalone editor, not a
reimplementation of it. Its theme is applied the way its own `main()` applies it — to the
QApplication — so all 5 of its themes are app-wide and the launcher adopts the same palette.

Two facts this module has to be honest about:

* The editor's core imports numpy at module level, so this interpreter must provide it (see
  requirements.txt). If that import does fail, the reason is kept in `_editor_error` and stays
  visible: a tab showing a healthy save list with nothing behind it is worse than one that fails
  loudly.
* The editor edits PSX memory-card payloads (PAYLOAD_SIZE bytes, with a DMW3 tag). The recomp's
  own `saveN.sav` slots are **not** that format — they are its private save (10,060 bytes here),
  with no tag. Those are listed but marked not editable, and opening one explains why instead of
  raising at a file the list implied was fine.
"""
from __future__ import annotations

import pathlib
import sys
import traceback
import weakref

from PySide6.QtCore import Qt
from PySide6.QtWidgets import (QApplication, QFileDialog, QFrame, QGroupBox, QHBoxLayout, QLabel,
                               QListWidget, QListWidgetItem, QMessageBox, QPushButton,
                               QScrollArea, QVBoxLayout, QWidget)

from .. import paths, runtime, saves
from . import theme

# Tabs currently embedding the editor. dmw3editor's theme picker applies a theme to the whole
# QApplication, so every live embed must re-scope itself when that happens. Weak references, so a
# closed window never keeps a tab alive.
_LIVE_TABS: list = []


def _register(tab) -> None:
    _LIVE_TABS.append(weakref.ref(tab))


class MemcardTab(QWidget):
    def __init__(self, cfg: dict) -> None:
        super().__init__()
        self.cfg = cfg
        self.root: pathlib.Path | None = paths.resolve_runtime(cfg)
        self.editor_root: pathlib.Path | None = paths.resolve_editor(cfg)
        self._editor = None
        self._editor_widget = None
        self._editor_error: str | None = None
        self._mc = None
        self._DMW3Save = None
        self._SaveError = Exception
        self._payload_size: int | None = None
        self._ed_theme = None
        _register(self)

        lay = QVBoxLayout(self)
        lay.setContentsMargins(theme.PAGE_MARGIN, theme.CARD_GAP,
                               theme.PAGE_MARGIN, theme.CARD_GAP)
        lay.setSpacing(theme.CARD_GAP)

        head = QHBoxLayout()
        head.setSpacing(theme.ROW_GAP)
        self.lbl_status = QLabel()
        self.lbl_status.setWordWrap(True)
        head.addWidget(self.lbl_status, 1)
        self.btn_rescan = QPushButton("Rescan")
        self.btn_rescan.clicked.connect(self.rescan)
        self.btn_addcard = QPushButton("Open memory card…")
        self.btn_addcard.clicked.connect(self.open_card_dialog)
        head.addWidget(self.btn_rescan)
        head.addWidget(self.btn_addcard)
        lay.addLayout(head)

        box = QGroupBox("Saves found")
        self.box = box
        bl = QVBoxLayout(box)
        bl.setContentsMargins(theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD)
        bl.setSpacing(theme.ROW_GAP)
        self.list_saves = QListWidget()
        # compact on purpose: the embedded editor below is this tab's content and should get the
        # space, while still listing every save found and which of them the editor can open
        self.list_saves.setMinimumHeight(66)
        self.list_saves.setMaximumHeight(92)
        self.list_saves.itemActivated.connect(lambda _i: self.open_selected())
        self.list_saves.currentRowChanged.connect(self._on_select)
        bl.addWidget(self.list_saves)
        btns = QHBoxLayout()
        btns.setSpacing(theme.ROW_GAP)
        self.btn_open = QPushButton("Open selected in editor")
        self.btn_open.setObjectName("primary")
        self.btn_open.clicked.connect(self.open_selected)
        self.btn_reveal = QPushButton("Show in folder")
        self.btn_reveal.clicked.connect(self._reveal)
        btns.addWidget(self.btn_open)
        btns.addWidget(self.btn_reveal)
        btns.addStretch(1)
        bl.addLayout(btns)
        lay.addWidget(box)

        # The embedded editor keeps its own minimum size (980x640) and scrolls inside the tab.
        # Squeezing it instead would clip its pages, because the editor's own content is not
        # inside a scroll area of its own.
        self.view = QScrollArea()
        self.view.setWidgetResizable(True)
        self.view.setFrameShape(QFrame.Shape.NoFrame)
        self.host = QWidget()
        self.host_lay = QVBoxLayout(self.host)
        self.host_lay.setContentsMargins(0, 0, 0, 0)
        self.view.setWidget(self.host)
        lay.addWidget(self.view, 1)

        self._load_editor()
        self.rescan()

    # ------------------------------------------------------------------ editor
    def _load_editor(self) -> None:
        self._editor_error = None
        if not self.editor_root:
            self._editor_error = ("Save editor not found. Use “Locate editor…” to point at the "
                                  "save editor project folder.")
            self.lbl_status.setText(self._editor_error)
            return
        if str(self.editor_root) not in sys.path:
            sys.path.insert(0, str(self.editor_root))
        try:
            from dmw3editor.core import memcard as mc
            from dmw3editor.core.save import DMW3Save, SaveError, PAYLOAD_SIZE
            from dmw3editor.ui import theme as ed_theme
            from dmw3editor.ui.main_window import MainWindow
            from dmw3editor.ui.themes_tab import saved_theme
        except Exception as exc:  # noqa: BLE001
            # The editor's core imports numpy at module level. Keeping the reason (rather than
            # only writing it to the label, which a later scan overwrote) is the point.
            self._editor_error = (f"Could not import the save editor from {self.editor_root}: "
                                  f"{type(exc).__name__}: {exc}")
            self.lbl_status.setText(self._editor_error)
            return

        self._mc, self._DMW3Save, self._SaveError = mc, DMW3Save, SaveError
        self._payload_size = PAYLOAD_SIZE
        self._ed_theme = ed_theme
        try:
            app = QApplication.instance()
            # The editor's own startup (dmw3editor.ui.main_window.main), minus the separate
            # window: its theme goes on the QApplication, so its 5 themes are app-wide and the
            # launcher adopts the same palette. One look for the whole app.
            ed_theme.apply(app, saved_theme())
            self._editor = MainWindow()
            # A QMainWindow, so its menu bar comes with it: embedding only the central widget
            # dropped that chrome, which is most of why the tab did not match standalone.
            self._editor.setWindowFlags(Qt.Widget)   # a child, not a second top-level window
            self._editor.setParent(self.host)
            self.host_lay.addWidget(self._editor)
            self._editor_widget = self._editor
            self._drop_editor_theme_page()
            self._adopt_editor_theme()
            self._hook_theme_apply()
        except Exception as exc:  # noqa: BLE001
            self._editor_error = f"Could not build the editor window: {type(exc).__name__}: {exc}"
            self.lbl_status.setText(f"{self._editor_error}\n{traceback.format_exc(limit=2)}")

    def _drop_editor_theme_page(self) -> None:
        """Strip the embedded editor's own theme chooser from THIS tab.

        The launcher's Settings page owns the app-wide theme picker now (the "Appearance" group
        with the Theme dropdown), so the editor's "APPEARANCE > Themes" page inside the Memory Card
        tab is a duplicate the owner asked to drop from this tab. Only the instance the launcher
        embeds is trimmed: the editor is imported live, and nothing about the standalone editor's
        own UI changes, so its page is still there when the editor runs by itself.

        The removal is deliberately narrow. "themes" is the editor's LAST page, so dropping it
        keeps `_keys`, `_nav_buttons` and the page stack index-parallel; a middle page would
        silently shift every later nav key. Everything is read through getattr and the whole thing
        no-ops if the editor's shape changes, so an editor update degrades to "the page is back",
        never to a crash.
        """
        ed = self._editor
        keys = getattr(ed, "_keys", None)
        buttons = getattr(ed, "_nav_buttons", None)
        if not keys or not buttons or "themes" not in keys:
            return
        idx = keys.index("themes")
        if idx != len(keys) - 1:          # only safe while it is the last page
            return
        btn = buttons[idx]

        # the nav button and the "APPEARANCE" section caption that introduces it
        lay = getattr(ed, "sidebar_layout", None)
        if lay is not None:
            pos = lay.indexOf(btn)
            if pos > 0:
                caption = lay.itemAt(pos - 1).widget()
                if caption is not None and caption.property("role") == "section":
                    lay.removeWidget(caption)
                    caption.deleteLater()
            lay.removeWidget(btn)
        group = getattr(ed, "_nav_group", None)
        if group is not None:
            group.removeButton(btn)
        btn.setParent(None)
        btn.deleteLater()

        # the page itself (its cards, their click handlers and the "Pick a color theme…" note
        # all go with the widget)
        page = getattr(ed, "themes_tab", None)
        stack = getattr(ed, "stack", None)
        if page is not None and stack is not None:
            stack.removeWidget(page)
            page.setParent(None)
            page.deleteLater()
            try:
                ed.themes_tab = None
            except Exception:          # noqa: BLE001  (a read-only attr just stays as it was)
                pass

        # keep the two lists parallel to the now-shorter stack
        keys.pop(idx)
        buttons.pop(idx)

    # ------------------------------------------------------------------ theming
    def _adopt_editor_theme(self) -> None:
        """Mirror one theme across both halves of the app.

        Nothing is set on the editor widget itself, deliberately: in standalone the editor's
        styling comes from the QApplication (its theme's palette and stylesheet) and the launcher
        keeps its own sheet off every ancestor of the editor (see theme.tabbar_stylesheet and
        MemcardTab.style_chrome). That leaves the embedded editor resolving its styles exactly the
        way it does standalone. Setting a palette here instead would pin the editor to a copy of
        the palette at that moment (Qt marks the widget WA_SetPalette) and it would then keep the
        old colours after a theme switch.
        """
        from . import theme as launcher_theme
        if self._editor is None or self._ed_theme is None:
            return
        pal = self._ed_theme.PALETTES.get(self._ed_theme.active_theme())
        if pal:
            launcher_theme.set_editor_active(True)
            launcher_theme.use_editor_theme(pal)

    def style_chrome(self, css: str) -> None:
        """Style this tab's OWN widgets, never an ancestor of the embedded editor.

        The launcher's sheet cannot go on the tab itself, because the tab contains the editor and
        Qt would let the launcher's rules reach inside it (see theme.tabbar_stylesheet).
        """
        for w in (self.lbl_status, self.btn_rescan, self.btn_addcard, self.box):
            try:
                w.setStyleSheet(css)
            except RuntimeError:
                pass

    def _hook_theme_apply(self) -> None:
        """Keep the launcher in step when a theme is picked inside the embedded editor.

        dmw3editor's picker calls theme.apply(QApplication.instance(), key), installing a new
        palette and stylesheet app-wide. The launcher is window-styled, so it does not follow on
        its own: this wrapper re-scopes every live embed, which is what recolours the launcher.
        """
        theme_mod = self._ed_theme
        if theme_mod is None or getattr(theme_mod, "_dmw3_launcher_hooked", None):
            return
        original = theme_mod.apply

        def apply_everywhere(app, name=None):
            original(app, name)
            alive = []
            for ref in list(_LIVE_TABS):
                tab = ref()
                if tab is None:
                    continue
                alive.append(ref)
                try:
                    tab._adopt_editor_theme()
                except RuntimeError:      # that tab's editor is already gone
                    pass
            _LIVE_TABS[:] = alive

        theme_mod.apply = apply_everywhere
        theme_mod._dmw3_launcher_hooked = True

    # ------------------------------------------------------------------ saves
    def rescan(self) -> None:
        self.root = paths.resolve_runtime(self.cfg)
        self.list_saves.clear()
        if not self.root:
            self._set_status("No install located yet — the Play tab finds it.")
            return
        found = saves.discover(self.root)
        editable_any = False
        for f in found:
            kind = "card" if f.is_card else "slot"
            size = f"{f.size:,} B"
            editable, note = self._classify(f)
            editable_any = editable_any or editable
            suffix = "" if editable else "   ·   not editable here"
            it = QListWidgetItem(f"{f.label}   ·   {size}{suffix}")
            it.setData(Qt.ItemDataRole.UserRole, (kind, str(f.path), editable, note))
            it.setToolTip(note or "")
            self.list_saves.addItem(it)
        where = paths.savedata_dir(self.root)
        n_slot = sum(1 for f in found if not f.is_card)
        n_card = sum(1 for f in found if f.is_card)

        # No dead UI: when nothing found is something the editor can open, the list is hidden and
        # the situation is stated in one line, leaving the whole tab to the editor. It comes back
        # by itself as soon as a file the editor can actually read exists.
        self.box.setVisible(editable_any)
        if not editable_any:
            what = f"{n_slot} save slot(s) in {where}" if n_slot else ""
            if n_card:
                what = (what + "   ·   " if what else "") + f"{n_card} memory card(s)"
            self._editor_hint = (
                (what + "   ·   " if what else "")
                + "nothing here is a PSX card payload the editor can edit. Use “Open memory "
                  "card…” to open a .mcr.")
            self._set_status(self._editor_hint)
            self.btn_open.setEnabled(False)
            return

        tail = ("  Opening a save here edits the same file the game reads."
                if self._editor is not None else "")
        self._set_status(f"{n_slot} save slot(s) in {where}   ·   {n_card} memory card(s) found."
                         + tail)
        self._on_select(self.list_saves.currentRow())

    def _set_status(self, scan: str) -> None:
        """One place that decides what the tab says, so a scan can never mask a dead editor."""
        if self._editor_error:
            self.lbl_status.setText(f"{scan}\n⚠ Save editor unavailable — {self._editor_error}")
            self.btn_open.setEnabled(False)
        else:
            self.lbl_status.setText(scan)

    def _classify(self, f: saves.Found) -> tuple[bool, str]:
        """Can the editor actually open this file?

        Memory cards can be probed. The recomp's own slots cannot: they are its private save
        format, not the PSX card payload the editor reads, so say so up front rather than letting
        the user click and get an exception.
        """
        if f.is_card:
            return True, "PS1 memory card — the Digimon World 3 entry inside it is opened."
        if self._payload_size is None:
            return True, ""  # editor unavailable, so cannot judge: do not block the click
        if f.size == self._payload_size:
            return True, f"Bare {self._payload_size:,}-byte payload, exactly what the editor reads."
        return False, (
            f"{f.path.name} is {f.size:,} bytes of the recomp's own save format, which the editor "
            f"cannot read: it edits PSX card payloads ({self._payload_size:,} bytes, with a DMW3 "
            f"tag at 0x204). Use “Open memory card…” for a .mcr card instead.")

    def _on_select(self, row: int) -> None:
        it = self.list_saves.item(row) if row is not None and row >= 0 else None
        if it is None:
            return
        _kind, _p, editable, note = it.data(Qt.ItemDataRole.UserRole)
        self.btn_open.setEnabled(bool(editable) and self._editor is not None)
        if note:
            self.lbl_status.setText(note)

    def _selected(self) -> tuple[str, pathlib.Path] | None:
        it = self.list_saves.currentItem()
        if not it:
            return None
        kind, p, _editable, _note = it.data(Qt.ItemDataRole.UserRole)
        return kind, pathlib.Path(p)

    def _reveal(self) -> None:
        sel = self._selected()
        if sel:
            runtime.open_path(sel[1])

    def open_selected(self) -> None:
        sel = self._selected()
        if not sel:
            QMessageBox.information(self, "No save", "Pick a save from the list first.")
            return
        kind, path = sel
        self.open_path(kind, path)

    def open_card_dialog(self) -> None:
        path, _ = QFileDialog.getOpenFileName(
            self, "Open a PS1 memory card", "",
            "Memory cards (*.mcr *.mcd *.mc *.mcs);;All files (*)")
        if not path:
            return
        self.open_path("card", pathlib.Path(path))

    def open_path(self, kind: str, path: pathlib.Path) -> None:
        """Load `path` into the embedded editor — the same sequence the editor's own
        open_file() performs, minus the dialog."""
        if self._editor is None:
            self._load_editor()
        if self._editor is None:
            QMessageBox.critical(
                self, "Editor unavailable",
                self._editor_error or "The save editor could not be loaded.")
            return
        try:
            if kind == "card":
                card = self._mc.MemoryCard.load(str(path))
                loc = card.find_dmw3_save()
                if loc is None:
                    raise self._SaveError(
                        "No Digimon World 3 save on this card. Expected an entry named "
                        "BASLUS-01436…, BESLES-03936… or BISLPS-03050…")
                save = self._DMW3Save(card.extract_payload(loc))
                self._editor._card, self._editor._loc = card, loc
            else:
                # A bare payload file. The recomp's own saveN.sav are NOT one: they are its own
                # format and the editor rejects them, so explain instead of raising at a file the
                # list implied was fine.
                payload = path.read_bytes()
                if self._payload_size is not None and len(payload) != self._payload_size:
                    self.lbl_status.setText(
                        f"{path.name} is {len(payload):,} bytes of the recomp's own save format. "
                        f"The editor edits PSX card payloads ({self._payload_size:,} bytes), so "
                        f"this file cannot be opened. Use “Open memory card…” for a .mcr.")
                    return
                save = self._DMW3Save(payload)
                self._editor._card, self._editor._loc = None, None
            self._editor._save = save
            self._editor._path = pathlib.Path(path)
            self._editor._dirty = False
            for hook in ("_refresh_titles",):
                fn = getattr(self._editor, hook, None)
                if callable(fn):
                    fn()
            self.lbl_status.setText(f"Editing {path.name}  ·  {path.parent}")
        except Exception as exc:  # noqa: BLE001
            QMessageBox.critical(self, "Could not open save",
                                 f"{exc}\n\n{traceback.format_exc(limit=2)}")
