"""The Decompilation tab: a read-only view of the 100% complete decomp tree.

Read-only is the contract, not a slogan: this module opens files for reading and never writes,
creates or deletes anything, and the tab exposes no edit action. `verify_launcher.py` proves it by
hashing the real tree before and after the tab is driven.

The progress panel copies the shape decomp.dev uses - one big percentage, a code bar and a data
bar, then the per-category breakdown - because that is the number the project publishes, and it
comes from the tree's own README rather than from a counter of ours (see `dmw3launcher/decomp.py`
for why). Every version the tree targets gets a segment of its own, USA, Europe and Japan alike.
USA and Europe carry rows in the README's numeric table. Japan has no row there, so its figures are
fetched off the GUI thread from what decomp.dev publishes for SLPS_034.46: the machine-readable
report behind the version's page (preferred - it carries the counts and the per-category breakdown,
which is what lets Japan show the SAME four rows the other two show, grouped the way this tree
groups its units), and the badges its README row links as the fallback. When neither can be read the
panel says the figures are unavailable instead of showing a stale or invented number. Switching
regions redraws EVERY part of the panel, so no sentence, bar or figure from one version survives
into another's.
"""
from __future__ import annotations

import threading

from PySide6.QtCore import QObject, QPoint, QRect, QRegularExpression, QSize, Qt, Signal
from PySide6.QtGui import (QColor, QFont, QFontDatabase, QPainter, QPen, QSyntaxHighlighter,
                           QTextCharFormat, QTextCursor, QTextDocument, QTextFormat)
from PySide6.QtWidgets import (QButtonGroup, QCheckBox, QFrame, QGridLayout, QGroupBox,
                               QHBoxLayout, QLabel, QLineEdit, QListWidget, QListWidgetItem,
                               QPlainTextEdit, QPushButton, QSplitter, QStackedWidget, QTabBar,
                               QTextEdit, QTreeWidget, QTreeWidgetItem, QVBoxLayout, QWidget)

from .. import decomp
from . import theme

MONO_FAMILIES = ("Cascadia Mono", "Consolas", "DejaVu Sans Mono", "Liberation Mono", "Courier New")
GUTTER_PAD = 10
SEGMENT_H = 30
MAX_TABS = 12        # open files kept at once; the oldest tab closes to make room
# USA leads Europe leads Japan in every region list on this tab: the same order the build list uses.
# The rank and the button list both come from the target table in `decomp`, so a version can never
# be ranked here and missing there (or the other way round).
REGION_RANK = {region: index for index, region in enumerate(decomp.REGION_ORDER)}


def mono_font(size: int = 11) -> QFont:
    available = set(QFontDatabase.families())
    for family in MONO_FAMILIES:
        if family in available:
            f = QFont(family, size)
            break
    else:
        f = QFontDatabase.systemFont(QFontDatabase.SystemFont.FixedFont)
        f.setPointSize(size)
    f.setStyleHint(QFont.StyleHint.Monospace)
    return f


def human_size(n: int) -> str:
    if n < 1024:
        return f"{n} B"
    if n < 1024 * 1024:
        return f"{n / 1024:.1f} KB"
    return f"{n / (1024 * 1024):.1f} MB"


# --------------------------------------------------------------------------- small widgets


class Bar(QWidget):
    """A flat progress bar, drawn from palette tokens so it follows a theme swap."""

    def __init__(self, height: int = 8, token: str = "OK", parent=None) -> None:
        super().__init__(parent)
        self._frac = 1.0
        self._token = token
        self.setFixedHeight(height)
        self.setMinimumWidth(60)

    def set_value(self, frac: float) -> None:
        self._frac = max(0.0, min(1.0, frac))
        self.update()

    def paintEvent(self, event) -> None:  # noqa: N802 (Qt naming)
        p = QPainter(self)
        p.setRenderHint(QPainter.RenderHint.Antialiasing, True)
        r = self.rect()
        p.setPen(Qt.PenStyle.NoPen)
        p.setBrush(QColor(theme.PANEL_HI))
        p.drawRoundedRect(r, r.height() / 2, r.height() / 2)
        filled = int(r.width() * self._frac)
        if filled > 0:
            p.setBrush(QColor(getattr(theme, self._token)))
            p.drawRoundedRect(0, 0, max(filled, r.height()), r.height(),
                              r.height() / 2, r.height() / 2)
        p.end()


class CHighlighter(QSyntaxHighlighter):
    """C colouring for the viewer, rebuilt from live palette tokens on every theme swap."""

    KEYWORDS = ("auto break case char const continue default do double else enum extern float for "
                "goto if inline int long register restrict return short signed sizeof static struct "
                "switch typedef union unsigned void volatile while").split()
    TYPES = ("s8 s16 s32 s64 u8 u16 u32 u64 f32 f64 bool BOOL TRUE FALSE NULL VECTOR SVECTOR CVECTOR "
             "MATRIX RECT GsOT GsOT_TAG PACKET GsIMAGE GsCELL u_char u_short u_long s_char").split()

    def __init__(self, document: QTextDocument) -> None:
        super().__init__(document)
        self._rules: list[tuple[QRegularExpression, QTextCharFormat]] = []
        self._comment = QTextCharFormat()
        self.rebuild()

    def _fmt(self, colour: str, bold: bool = False, italic: bool = False) -> QTextCharFormat:
        f = QTextCharFormat()
        f.setForeground(QColor(colour))
        f.setFontWeight(QFont.Weight.DemiBold if bold else QFont.Weight.Normal)
        f.setFontItalic(italic)
        return f

    def rebuild(self) -> None:
        """Rebuild the rule set from the active palette. Cheap; called on every theme change."""
        pal = theme.code_palette()
        self._rules = [
            (QRegularExpression(r"\b(" + "|".join(self.KEYWORDS) + r")\b"),
             self._fmt(pal["keyword"], bold=True)),
            (QRegularExpression(r"\b(" + "|".join(self.TYPES) + r")\b"), self._fmt(pal["type"])),
            (QRegularExpression(r"^\s*#\s*\w+"), self._fmt(pal["preproc"])),
            (QRegularExpression(r"\b[A-Za-z_]\w*(?=\s*\()"), self._fmt(pal["func"])),
            (QRegularExpression(r"\b(0[xX][0-9a-fA-F]+|\d+\.?\d*[fFuUlL]*)\b"),
             self._fmt(pal["number"])),
            # Strings and comments last: they win over anything inside them.
            (QRegularExpression(r"\"(\\\\.|[^\"\\\\])*\""), self._fmt(pal["string"], italic=True)),
            (QRegularExpression(r"'(\\\\.|[^'\\\\])*'"), self._fmt(pal["string"], italic=True)),
            (QRegularExpression(r"//[^\n]*"), self._fmt(pal["comment"], italic=True)),
        ]
        self._comment = self._fmt(pal["comment"], italic=True)
        self.rehighlight()

    def highlightBlock(self, text: str) -> None:  # noqa: N802 (Qt naming)
        for pattern, fmt in self._rules:
            it = pattern.globalMatch(text)
            while it.hasNext():
                m = it.next()
                self.setFormat(m.capturedStart(), m.capturedLength(), fmt)
        # /* ... */ possibly spanning blocks, tracked with the block state.
        start = 0
        if self.previousBlockState() == 1:
            end = text.find("*/")
            if end == -1:
                self.setFormat(0, len(text), self._comment)
                self.setCurrentBlockState(1)
                return
            self.setFormat(0, end + 2, self._comment)
            start = end + 2
        while True:
            begin = text.find("/*", start)
            if begin == -1:
                self.setCurrentBlockState(0)
                break
            end = text.find("*/", begin + 2)
            if end == -1:
                self.setFormat(begin, len(text) - begin, self._comment)
                self.setCurrentBlockState(1)
                break
            self.setFormat(begin, end - begin + 2, self._comment)
            start = end + 2


class _Gutter(QWidget):
    """Line numbers beside the code view."""

    def __init__(self, view: "CodeView") -> None:
        super().__init__(view)
        self._view = view
        self.setObjectName("codeGutter")

    def sizeHint(self) -> QSize:  # noqa: N802 (Qt naming)
        return QSize(self._view.gutter_width(), 0)

    def paintEvent(self, event) -> None:  # noqa: N802 (Qt naming)
        pal = theme.code_palette()
        p = QPainter(self)
        p.fillRect(self.rect(), QColor(pal["gutter_bg"]))
        p.setPen(QColor(pal["gutter_fg"]))
        p.setFont(self._view.font())
        current = self._view.textCursor().blockNumber() + 1
        block = self._view.firstVisibleBlock()
        top = self._view.blockBoundingGeometry(block).translated(self._view.contentOffset()).top()
        number = block.blockNumber() + 1
        width = self._view.gutter_width() - GUTTER_PAD
        while block.isValid() and top <= event.rect().bottom():
            if block.isVisible():
                # The line the caret is on reads brighter, as it does in an editor.
                p.setPen(QColor(pal["gutter_active"] if number == current else pal["gutter_fg"]))
                p.drawText(0, int(top), width, self._view.fontMetrics().height(),
                           int(Qt.AlignmentFlag.AlignRight | Qt.AlignmentFlag.AlignVCenter),
                           str(number))
            block = block.next()
            top += self._view.blockBoundingRect(block.previous()).height()
            number += 1
        p.end()


class _FileTabBar(QTabBar):
    """A tab strip whose close glyphs sit evenly inside their tabs.

    Qt's own close button is laid out flush against the tab's border, so the space lands on the
    wrong side: measured on a real tab, 4px from the glyph to the edge against a 19px gap to the
    name's end. A stylesheet cannot move it (the sheet's padding moves the label, not the button),
    and a button widget does not help either: Qt reserves a fixed spacing before any tab button, so
    the best a widget gives is 16px on one side and 11px on the other. Painting the glyph is what
    gives exact numbers, with the tab's padding-right reserving the room it lives in.
    """

    closeRequested = Signal(int)      # tab index

    GLYPH = 7        # the X's width in pixels
    OFFSET = 13      # its centre, measured from the tab's right edge
    HIT = 9          # half-size of the clickable box around it

    def __init__(self, parent=None) -> None:
        super().__init__(parent)
        # Without this, Qt delivers NO mouse-move events while no button is held, so the glyph's
        # hover state and cursor would never fire for a real user (only clicks would).
        self.setMouseTracking(True)
        self._hover = -1

    def close_rect(self, index: int) -> QRect:
        rect = self.tabRect(index)
        centre = QPoint(int(rect.right() - self.OFFSET), int(rect.center().y()))
        return QRect(centre.x() - self.HIT, centre.y() - self.HIT, self.HIT * 2, self.HIT * 2)

    def _index_at(self, point: QPoint) -> int:
        for index in range(self.count()):
            if self.close_rect(index).contains(point):
                return index
        return -1

    def paintEvent(self, event) -> None:  # noqa: N802 (Qt naming)
        super().paintEvent(event)
        p = QPainter(self)
        p.setRenderHint(QPainter.RenderHint.Antialiasing, True)
        for index in range(self.count()):
            box = self.close_rect(index)
            hovered = index == self._hover
            if hovered:
                p.setPen(Qt.PenStyle.NoPen)
                p.setBrush(QColor(theme.code_palette()["current_line"]))
                p.drawRoundedRect(box, 4, 4)
            pen = QPen(QColor(theme.TEXT if hovered else theme.TEXT_DIM), 1.3)
            pen.setCapStyle(Qt.PenCapStyle.RoundCap)
            p.setPen(pen)
            centre, half = box.center(), self.GLYPH // 2
            p.drawLine(centre.x() - half, centre.y() - half, centre.x() + half, centre.y() + half)
            p.drawLine(centre.x() + half, centre.y() - half, centre.x() - half, centre.y() + half)
        p.end()

    def mousePressEvent(self, event) -> None:  # noqa: N802 (Qt naming)
        index = self._index_at(event.position().toPoint())
        if index >= 0 and event.button() == Qt.MouseButton.LeftButton:
            self.closeRequested.emit(index)      # closing must not also select the tab
            event.accept()
            return
        super().mousePressEvent(event)

    def mouseMoveEvent(self, event) -> None:  # noqa: N802 (Qt naming)
        index = self._index_at(event.position().toPoint())
        if index != self._hover:
            self._hover = index
            self.setCursor(Qt.CursorShape.PointingHandCursor if index >= 0
                           else Qt.CursorShape.ArrowCursor)
            self.update()
        super().mouseMoveEvent(event)

    def leaveEvent(self, event) -> None:  # noqa: N802 (Qt naming)
        if self._hover != -1:
            self._hover = -1
            self.update()
        super().leaveEvent(event)


class CodeView(QPlainTextEdit):
    """Read-only code with a line-number gutter. No edit affordances at all."""

    def __init__(self, parent=None) -> None:
        super().__init__(parent)
        self.setObjectName("codeView")
        self.setReadOnly(True)
        self.setUndoRedoEnabled(False)
        self.setLineWrapMode(QPlainTextEdit.LineWrapMode.NoWrap)
        self.setFont(mono_font())
        self.setTabStopDistance(self.fontMetrics().horizontalAdvance(" ") * 4)
        self.setMinimumHeight(200)
        self._gutter = _Gutter(self)
        self.highlighter = CHighlighter(self.document())
        self.blockCountChanged.connect(self._update_margins)
        self.updateRequest.connect(self._on_update_request)
        self.cursorPositionChanged.connect(self._highlight_current_line)
        self._update_margins()
        self._highlight_current_line()

    def _highlight_current_line(self) -> None:
        """Band the caret's line across the full width, the way an editor does."""
        selection = QTextEdit.ExtraSelection()
        selection.format.setBackground(QColor(theme.code_palette()["current_line"]))
        selection.format.setProperty(QTextFormat.Property.FullWidthSelection, True)
        selection.cursor = self.textCursor()
        selection.cursor.clearSelection()
        self.setExtraSelections([selection])
        self._gutter.update()

    # -- gutter plumbing (the Qt code-editor pattern)
    def gutter_width(self) -> int:
        digits = max(3, len(str(max(1, self.blockCount()))))
        return GUTTER_PAD + self.fontMetrics().horizontalAdvance("9") * digits

    def _update_margins(self) -> None:
        self.setViewportMargins(self.gutter_width(), 0, 0, 0)

    def _on_update_request(self, rect, dy: int) -> None:
        if dy:
            self._gutter.scroll(0, dy)
        else:
            self._gutter.update(0, rect.y(), self._gutter.width(), rect.height())
        if rect.contains(self.viewport().rect()):
            self._update_margins()

    def resizeEvent(self, event) -> None:  # noqa: N802 (Qt naming)
        super().resizeEvent(event)
        cr = self.contentsRect()
        self._gutter.setGeometry(cr.left(), cr.top(), self.gutter_width(), cr.height())

    def restyle(self) -> None:
        self.highlighter.rebuild()
        self._gutter.update()

    # -- content
    def load(self, rel: str, line: int | None = None) -> bool:
        try:
            lines, truncated = decomp.read_lines(rel)
        except (OSError, FileNotFoundError):
            return False
        self.setPlainText("\n".join(lines))
        if truncated:
            self.appendPlainText(f"\n… truncated at {decomp.LINE_CAP:,} lines by the viewer.")
        if line:
            self.goto_line(line)
        else:
            self.moveCursor(QTextCursor.MoveOperation.Start)
        return True

    def goto_line(self, line: int) -> None:
        block = self.document().findBlockByNumber(max(0, line - 1))
        if not block.isValid():
            return
        cursor = QTextCursor(block)
        cursor.select(QTextCursor.SelectionType.LineUnderCursor)
        self.setTextCursor(cursor)
        self.centerCursor()


# --------------------------------------------------------------------------- progress panel


class FigureFetch(QObject):
    """Fetches ONE version's decomp.dev figures off the GUI thread and reports back by signal.

    The panel must never wait on decomp.dev while the window is painting, so the round trip happens
    on its own thread. WHICH URLs are read (the version's report, or the badges its README row links)
    is `decomp.published_figures`'s decision, made inside the worker: the tab only says which version
    it wants. `done` is emitted from that thread and QUEUED to the thread that owns this object (the
    GUI thread), which is the only reason it is safe to touch widgets in the slot - the worker itself
    returns data and nothing else.
    """

    done = Signal(str, dict)          # version serial, its published figures

    def __init__(self, serial: str, fetch=None, cache_file=None, parent=None) -> None:
        super().__init__(parent)
        self._serial = serial
        self._fetch = fetch
        self._cache_file = cache_file
        self._thread: threading.Thread | None = None

    def start(self) -> None:
        self._thread = threading.Thread(target=self._run, daemon=True,
                                        name=f"dw3-figures-{self._serial}")
        self._thread.start()

    def _run(self) -> None:
        try:
            figures = decomp.published_figures(self._serial, fetch=self._fetch,
                                               cache_file=self._cache_file)
        except Exception:              # noqa: BLE001 - a failed fetch is a state, not a crash
            figures = {}
        self.done.emit(self._serial, figures)


class ProgressCard(QFrame):
    """The decomp.dev-style progress panel: percentage, code and data bars, per-category rows."""

    def __init__(self, parent=None) -> None:
        super().__init__(parent)
        self.setObjectName("card")
        self._regions: dict[str, dict] = {}
        # Versions with no published figures (and versions not in this tree at all): their segments
        # say so. See `set_status` and `_show_unpublished`.
        self._no_figures: dict[str, dict] = {}
        self._buttons: dict[str, QPushButton] = {}
        self._rows: list[tuple[QLabel, QLabel, Bar, QLabel]] = []
        # Figures fetched from decomp.dev for a version (its machine-readable report, or the badges
        # its README row links), by region; the region on screen; and the fetches still running.
        # `fetcher` replaces `decomp.fetch_text` - the tab's own tests inject one, so the network
        # never decides what a check sees.
        self._figures: dict[str, dict] = {}
        self._shown = ""
        self._fetching: set[str] = set()
        self.fetcher = None
        # Where a fetched badge is remembered between runs. None = the launcher's own per-user cache
        # file; the tab's checks point it at a temporary one so a test's canned badge can never
        # become a figure the real panel later shows.
        self.cache_file = None
        # The project's own "source: … · compiler: …" line, used by every region the numeric table
        # covers; the badge-published ones carry a line of their own. Set by `set_status`, and empty
        # until then so a paint before the first scan cannot raise.
        self._project_basis = ""
        outer = QVBoxLayout(self)
        outer.setContentsMargins(theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD)
        outer.setSpacing(theme.ROW_GAP)

        self.title = QLabel("Progress")
        self.title.setObjectName("section")
        outer.addWidget(self.title)

        body = QHBoxLayout()
        body.setSpacing(theme.CARD_GAP)

        # The region switch spans the card's own width, above the two columns: three segments
        # (USA, Europe, Japan) do not fit the left column at the window's minimum width, and Qt
        # then shrinks them below their own labels - a clipped label is a bug the owner sees, not a
        # style choice. USA first, Europe second, Japan third: the one region order every list in
        # the launcher follows.
        switch = QHBoxLayout()
        switch.setSpacing(theme.ROW_GAP)
        self._group = QButtonGroup(self)
        self._group.setExclusive(True)
        # The height is a MINIMUM, not a fixed size: the #segment sheet's own padding needs more
        # than 30px once the labels carry their percentage, and a fixed 30 was ignored while the
        # layout still reserved only 30: which is what let the buttons overrun the headline below.
        for region in decomp.REGION_ORDER:
            b = QPushButton(region)
            b.setObjectName("segment")
            b.setCheckable(True)
            b.setMinimumHeight(SEGMENT_H)
            b.clicked.connect(lambda _=False, r=region: self.show_region(r))
            self._group.addButton(b)
            self._buttons[region] = b
            switch.addWidget(b)
        switch.addStretch(1)
        outer.addLayout(switch)

        # left: the headline number and the code/data bars
        left = QVBoxLayout()
        left.setSpacing(theme.ROW_GAP)

        self.percent = QLabel("-")
        self.percent.setObjectName("bigPct")
        left.addWidget(self.percent)
        self.counts = QLabel("")
        self.counts.setObjectName("dim")
        self.counts.setWordWrap(True)
        left.addWidget(self.counts)

        for label, token in (("code", "OK"), ("data", "OK")):
            row = QHBoxLayout()
            row.setSpacing(theme.ROW_GAP)
            tag = QLabel(label)
            tag.setObjectName("dim")
            tag.setFixedWidth(38)
            bar = Bar(8, token)
            value = QLabel("-")
            value.setFixedWidth(64)
            value.setObjectName("dim")
            row.addWidget(tag)
            row.addWidget(bar, 1)
            row.addWidget(value)
            left.addLayout(row)
            setattr(self, f"bar_{label}", bar)
            setattr(self, f"value_{label}", value)
        left.addStretch(1)
        body.addLayout(left, 3)

        # right: one row per category, the way decomp.dev breaks a unit down
        self._grid = QGridLayout()
        self._grid.setHorizontalSpacing(theme.ROW_GAP)
        self._grid.setVerticalSpacing(theme.ROW_GAP)
        self._grid.setColumnStretch(0, 1)
        body.addLayout(self._grid, 4)
        outer.addLayout(body)

        self.basis = QLabel("")
        self.basis.setObjectName("hint")
        self.basis.setWordWrap(True)
        outer.addWidget(self.basis)

    def set_status(self, published: dict, totals: tuple[int, int, int, int]) -> None:
        """Render the project's published numbers, or say plainly that they are ours instead."""
        self._regions = dict(published.get("regions", {}))
        # A version the README's table publishes no figures for (Japan today): a real region whose
        # numbers live somewhere else, so it gets a button that says where. A version that is not in
        # this tree at all says that instead, and its segment is disabled rather than being a click
        # that does nothing.
        self._no_figures = {t["region"]: t for t in decomp.regions()
                            if t["region"] not in self._regions}
        for region, button in self._buttons.items():
            if region in self._regions:
                button.setText(f"{region}  {self._regions[region]['percent']:.2f}%")
                button.setEnabled(True)
            elif region in self._no_figures:
                target = self._no_figures[region]
                # "no figures" only when the README offers nothing for the version; where its row
                # links decomp.dev badges the figures exist THERE, so the segment says where rather
                # than implying the project has none. Once those figures have been read, the segment
                # carries the number like the other two (see `_figures_arrived`).
                state = "not in this tree"
                if target["present"]:
                    state = "decomp.dev" if target.get("page") else "no figures"
                    # Figures already read for this version keep their number on the segment across
                    # a refresh, so the button never falls back to "decomp.dev" under a panel that
                    # is showing the percentage.
                    known = self._figures.get(region)
                    if known and self._headline(known) is not None:
                        state = f"{self._headline(known):.2f}%"
                button.setText(f"{region}  {state}")
                button.setEnabled(bool(target["present"]))
            else:
                button.setText(region)
                button.setEnabled(True)

        if not self._regions:
            self.title.setText("Progress, not published in this tree")
            self.percent.setText("-")
            files, lines, funcs, stubs = totals
            self.counts.setText(f"{files:,} files · {lines:,} lines of code · "
                                f"{funcs:,} functions counted · {stubs} stubs")
            self._set_bars(None, None)
            self._clear_rows()
            self.basis.setText("The README's progress table could not be read, so no percentage is "
                               "shown: a count of our own would be a guess, not the project's word.")
            return

        badges = published.get("badges", {})
        # One line, and only the badges that mean something here: the platform badge just repeats
        # the tab's own name and pushed the line to a third row.
        wanted = ("fake matches | hacks", "compiler", "versions")
        self._project_basis = " · ".join(
            [f"source: {published.get('source', 'README')}"]
            + [f"{k}: {badges[k]}" for k in wanted if badges.get(k)])
        self.show_region("USA" if "USA" in self._regions else next(iter(self._regions)))

    def _set_bars(self, code: float | None, data: float | None) -> None:
        """The code and data bars plus their percentages. None is 'not published', never a zero."""
        for bar, value_label, percent in ((self.bar_code, self.value_code, code),
                                          (self.bar_data, self.value_data, data)):
            bar.set_value(0.0 if percent is None else percent / 100.0)
            value_label.setText("-" if percent is None else f"{percent:.2f}%")

    def show_region(self, region: str) -> None:
        """Draw `region`'s page, start to finish.

        Every branch fills EVERY part of the panel - title, percentage, counts, both bars, the rows
        and the footnote - because a part left over from the version shown before is exactly the bug
        the owner saw (the Japan note sitting under the USA figures). Nothing here is carried across
        a region change.
        """
        self._shown = region
        if region in self._buttons:
            self._buttons[region].setChecked(True)
        bucket = self._regions.get(region)
        if bucket:
            self._show_table_region(bucket)
            return
        target = self._no_figures.get(region)
        if not target or not target["present"]:
            self._clear_page(region)
            return
        figures = self._figures.get(region)
        if figures:
            self._show_published_figures(region, target, figures)
        elif target.get("report") or target.get("measures"):
            # The figures are published upstream - as the report behind the version's page, or as the
            # badge images its README row links. Ask for them, off the GUI thread, and show a
            # placeholder that says so until the answer arrives.
            self._start_fetch(region, target)
        else:
            self._show_unpublished(region, target)

    def _show_table_region(self, bucket: dict) -> None:
        """USA and Europe: the README's own numeric table, breakdown and all."""
        self.title.setText("Progress, as published by the project")
        self.percent.setText(f"{bucket['percent']:.2f}%")
        self.counts.setText(f"{bucket['matched']:,} / {bucket['total']:,} functions matched")
        rows = [r for r in bucket["rows"] if r["label"].lower() != "total"]
        code = sum(r["code"] for r in rows) / len(rows) if rows else 0.0
        data = sum(r["data"] for r in rows) / len(rows) if rows else 0.0
        self._set_bars(code, data)
        self.basis.setText(self._project_basis)

        self._draw_rows(rows + [{"label": "Total", "matched": bucket["matched"],
                                 "total": bucket["total"]}])

    def _draw_rows(self, rows: list[dict]) -> None:
        """One row per part: its own label, its own counts, and the ratio between them.

        The same cells for every version that has a breakdown - the table's rows for USA and Europe,
        the report's grouped rows for Japan - so the three panels are the same shape. The last row is
        the total and is set in bold. `rows` empty clears the grid, which is what a version with no
        derivable breakdown gets: no rows at all, rather than a row of zeroes.
        """
        self._clear_rows()
        for index, row in enumerate(rows):
            name = QLabel(row["label"] + ("  (complete)" if row["matched"] == row["total"] else ""))
            name.setObjectName("dim")
            # The category names are the widest thing in the card; letting them wrap keeps the
            # card's minimum width small enough to fit the window instead of forcing a squeeze.
            name.setWordWrap(True)
            frac = (row["matched"] / row["total"]) if row["total"] else 0.0
            counts = QLabel(f"{row['matched']:,} / {row['total']:,}")
            bar = Bar(6)
            bar.set_value(frac)
            pct = QLabel(f"{100.0 * frac:.2f}%")
            pct.setObjectName("dim")
            pct.setFixedWidth(64)
            last = index == len(rows) - 1
            # Centre every cell vertically so a wrapped name does not push the bar out of line.
            for col, w in ((0, name), (1, counts), (2, bar), (3, pct)):
                self._grid.addWidget(w, index, col, Qt.AlignmentFlag.AlignVCenter)
                if last:
                    f = w.font()
                    f.setBold(True)
                    w.setFont(f)
            self._rows.append((name, counts, bar, pct))

    @staticmethod
    def _headline(figures: dict) -> float | None:
        """The one big number for a badge-published version: the first measure it publishes as a %.

        decomp.dev badges a version's `functions` as counts for some versions (SLPS_034.46 renders
        `3,606 / 3,606`) and as a percentage for others, so the headline is taken from the first of
        functions/code/data that IS a percentage, and is None when none of them is - never converted
        from a count, which would be a number the project did not publish as a percentage.
        """
        for measure in ("functions", "code", "data"):
            percent = (figures.get(measure) or {}).get("percent")
            if percent is not None:
                return float(percent)
        return None

    def _show_published_figures(self, region: str, target: dict, figures: dict) -> None:
        """A version the README's TABLE has no rows for, whose figures decomp.dev publishes.

        Japan is the case: decomp.dev carries SLPS_034.46, the README's version row links the three
        badge images AND the version's page, and every number here was read out of what that site
        serves - decomp.dev's own report for the version where one came back, the badge SVGs
        otherwise. So the big number, the two bars and the counts are the project's own. The per-part
        rows come from the report's own categories, grouped by `decomp.report_parts` the way THIS tree
        groups its units (the executable, the overlays `overlays.sha1` names, the stages its stage list
        carries) and drawn under the README table's own labels; where no such breakdown can be derived,
        no rows are drawn at all rather than rows of invented numbers.
        """
        percent = self._headline(figures)
        counts = (figures.get("functions") or {}).get("counts")
        segment = f"{region}  {percent:.2f}%" if percent is not None else f"{region}  decomp.dev"
        if region in self._buttons:
            self._buttons[region].setText(segment)
        self.title.setText("Progress, as published by the project")
        self.percent.setText("-" if percent is None else f"{percent:.2f}%")
        if counts:
            self.counts.setText(f"{counts[0]:,} / {counts[1]:,} functions matched")
        else:
            self.counts.setText(f"{target['binary']}, as published on decomp.dev: the README's own "
                                f"table carries no rows for this version")
        self._set_bars((figures.get("code") or {}).get("percent"),
                       (figures.get("data") or {}).get("percent"))
        parts = list(figures.get("parts") or [])
        if parts and counts:
            # The report's own total, as the last row - the table's own Total row for the others.
            parts.append({"label": "Total", "matched": counts[0], "total": counts[1]})
        self._draw_rows(parts)
        if figures.get("report_url"):
            self.basis.setText(self._report_basis(target, figures))
        else:
            self.basis.setText(
                f"README.md's version row links its decomp.dev page: {target['page']}. The figures "
                f"are read from the badge images that row links, not counted here.")

    def _report_basis(self, target: dict, figures: dict) -> str:
        """Where a report-read version's figures came from, and how its rows were grouped.

        Both facts are the point of the line: the URL and the upstream commit the report was generated
        from say which revision the numbers describe, and the grouping sentence says which parts of
        THIS tree each row stands for - so the row label "The stages (294)" is not a number the panel
        picked, it is the 294 stages `config/jp/stages.sha1` carries.
        """
        url = figures.get("report_url") or target.get("report_url") or target.get("page", "")
        commit = str(figures.get("commit") or "")
        text = (f"decomp.dev's own report for {target['binary']} ({url}"
                + (f", generated from upstream commit {commit[:7]}" if commit else "")
                + "): every count and percentage is the report's, read from the JSON it serves - "
                  "not counted here.")
        if figures.get("parts"):
            text += (f" Its categories are grouped the way this tree groups its units: the executable,"
                     f" the {target['overlays']} overlays {target['config']}/overlays.sha1 names, and"
                     f" the {target['stages']} stages its own stage list carries.")
        else:
            text += (" The report's per-part rows could not be grouped from this tree, so none are "
                     "shown.")
        return text

    def _start_fetch(self, region: str, target: dict) -> None:
        """Ask decomp.dev for this version's figures, without holding up the window."""
        if not (target.get("report") or target.get("measures")):
            self._figures[region] = {}
            self._show_unpublished(region, target)
            return
        self._show_checking(region, target)
        if region in self._fetching:
            return                       # one round trip per version at a time
        self._fetching.add(region)
        worker = FigureFetch(target["binary"], fetch=self.fetcher, cache_file=self.cache_file,
                             parent=self)
        worker.done.connect(self._figures_arrived)
        worker.start()

    def _figures_arrived(self, serial: str, figures: dict) -> None:
        """A fetch came back: draw it if that version is the one on screen, and say where it went."""
        region = next((r for r, t in self._no_figures.items() if t["binary"] == serial), None)
        if region is None:
            return
        self._fetching.discard(region)
        self._figures[region] = dict(figures or {})
        if figures and region in self._buttons:
            percent = self._headline(figures)
            if percent is not None:
                self._buttons[region].setText(f"{region}  {percent:.2f}%")
        if self._shown != region:
            return                       # the user moved on: this page is not the one on screen
        if figures:
            self._show_published_figures(region, self._no_figures[region], figures)
        else:
            self._show_unavailable(region, self._no_figures[region])

    def _show_checking(self, region: str, target: dict) -> None:
        """The placeholder while the figures are in flight: a state, not a number."""
        self.title.setText("Progress, published upstream")
        self.percent.setText("…")
        self.counts.setText(f"asking decomp.dev for {target['binary']}'s published figures…")
        self._set_bars(None, None)
        self._clear_rows()
        self.basis.setText(f"README.md's version row links its decomp.dev page: {target['page']}. "
                           f"The figures are read from decomp.dev - the report behind that page "
                           f"where there is one, the badge images it links otherwise - not counted "
                           f"here.")

    def _show_unavailable(self, region: str, target: dict) -> None:
        """The figures are published upstream, and this run could not read them.

        No network, a DNS failure, a timeout, a badge that renders no number: all the same answer.
        The percentage stays a dash - a remembered or made-up one would read as a measurement the
        project never published, which is worse than saying nothing.
        """
        self.title.setText("Progress, published upstream")
        self.percent.setText("-")
        self.counts.setText(f"{target['binary']}'s figures are published on decomp.dev and could not "
                            f"be read just now, so none are shown - no stale or made-up number")
        self._set_bars(None, None)
        self._clear_rows()
        self.basis.setText(self._where_text(target))

    def _show_unpublished(self, region: str, target: dict) -> None:
        """A version the tree carries that neither the table nor a badge page publishes figures for.

        Shown as exactly that: no percentage, no bar, no zero standing in for a number, and the
        project's own words for the version - the README row's prose where it has some.
        """
        self.title.setText("Progress, not published in this tree")
        self.percent.setText("-")
        self.counts.setText(f"{target['binary']} is in this tree, and the README's table "
                            f"publishes no figures for this version")
        self._set_bars(None, None)
        self._clear_rows()
        self.basis.setText(self._where_text(target))

    @staticmethod
    def _where_text(target: dict) -> str:
        """Where a version's state really lives, quoted rather than replaced. Never a figure."""
        if target.get("page"):
            where = f"README.md's version row links its decomp.dev page: {target['page']}"
        elif target.get("note"):
            where = f"README.md's version row says: “{target['note']}”"
        else:
            where = "README.md carries no figures and no note for this version"
        checklist = ""
        if target.get("overlays") or target.get("stages"):
            checklist = (f" The tree carries its own checklists ({target['config']}: "
                         f"{target['overlays']} overlays, {target['stages']} stages), which say "
                         f"what is expected of it, not how much is matched.")
        return where + "." + checklist

    def _clear_page(self, region: str) -> None:
        """A version with nothing published at all: said plainly, with no number anywhere."""
        self.title.setText("Progress, not published in this tree")
        self.percent.setText("-")
        self.counts.setText(f"{region} is not a version the README's table carries")
        self._set_bars(None, None)
        self._clear_rows()
        self.basis.setText("The README's progress table has no row for this version, so no "
                           "percentage is shown.")

    def _clear_rows(self) -> None:
        """Drop every breakdown row at once.

        `setParent(None)` as well as the deletion: a widget removed from a layout is still a child of
        this card and would go on PAINTING at its old geometry until the deferred delete is handled,
        so a region switch could flash the previous version's rows (and an offscreen grab would catch
        them). Detaching hides it at once; the deferred delete still frees it.
        """
        for name, counts, bar, pct in self._rows:
            for w in (name, counts, bar, pct):
                self._grid.removeWidget(w)
                w.setParent(None)
                w.deleteLater()
        self._rows = []


# --------------------------------------------------------------------------- the tab


class DecompTab(QWidget):
    """A read-only browser over the decompilation tree, with its published progress on top."""

    def __init__(self, cfg: dict | None = None, parent=None) -> None:
        super().__init__(parent)
        self.cfg = cfg or {}
        self._inventory: decomp.Inventory | None = None
        self._items: dict[str, QTreeWidgetItem] = {}
        # Set while open_path moves the tree's selection itself, so the selection handler does not
        # re-open the file without the line and undo a jump-to-line.
        self._loading = False
        self._docs: dict[str, CodeView] = {}     # open files, by path, one CodeView each
        self._order: list[str] = []              # their tab order
        self._build()

    # -- construction
    def _build(self) -> None:
        page = QVBoxLayout(self)
        page.setContentsMargins(theme.PAGE_MARGIN, theme.PAGE_MARGIN,
                                theme.PAGE_MARGIN, theme.PAGE_MARGIN)
        page.setSpacing(theme.CARD_GAP)

        head = QHBoxLayout()
        head.setSpacing(theme.ROW_GAP)
        title = QLabel("Decompilation")
        title.setObjectName("pageTitle")
        head.addWidget(title)
        head.addStretch(1)
        self.readonly = QLabel("read-only")
        self.readonly.setObjectName("hint")
        head.addWidget(self.readonly)
        page.addLayout(head)

        # The identity line gets its OWN full-width row rather than a cell beside the title: it is
        # long, and letting it wrap inside a narrow cell stacked it into three lines. Full width it
        # reads as one line, and its minimum width stays small so it cannot force a squeeze.
        self.identity = QLabel("")
        self.identity.setObjectName("dim")
        self.identity.setWordWrap(True)
        page.addWidget(self.identity)

        self.progress = ProgressCard()
        page.addWidget(self.progress)

        split = QSplitter(Qt.Orientation.Horizontal)
        split.setChildrenCollapsible(False)
        split.addWidget(self._left_panel())
        split.addWidget(self._right_panel())
        split.setStretchFactor(0, 1)
        split.setStretchFactor(1, 1)
        # Even split by default: the owner dragged the left side to roughly half and wants THAT as
        # the first-run look. Equal stretch factors keep the panes even when the window resizes,
        # and the splitter stays draggable; neither pane is collapsible, so the split stays a real
        # split down to the window's minimum width instead of collapsing a side.
        split.setSizes([560, 560])
        page.addWidget(split, 1)

        self.status = QLabel("")
        self.status.setObjectName("dim")
        self.status.setWordWrap(True)
        page.addWidget(self.status)

        theme.on_change(self._restyle)

    def _left_panel(self) -> QWidget:
        panel = QWidget()
        outer = QVBoxLayout(panel)
        outer.setContentsMargins(0, 0, 0, 0)
        outer.setSpacing(theme.ROW_GAP)

        top = QWidget()
        tl = QVBoxLayout(top)
        tl.setContentsMargins(0, 0, 0, 0)
        tl.setSpacing(theme.ROW_GAP)
        self.filter_box = QLineEdit()
        self.filter_box.setPlaceholderText("Filter files…")
        self.filter_box.textChanged.connect(self._apply_filter)
        tl.addWidget(self.filter_box)

        self.tree = QTreeWidget()
        self.tree.setObjectName("fileTree")
        self.tree.setHeaderLabels(["File", "Detail"])
        # The columns FIT the sidebar. A fixed 190px File column plus a 100px Detail column was
        # wider than the pane, so Qt raised a horizontal scrollbar that crowded the last row and
        # hard-cut the Detail text to "641 fi…". The File column only needs room for the longest
        # name; the Detail column takes what is left, so there is no scrollbar and the rows share
        # one clean edge. A name longer than its column elides rather than making the tree scroll.
        self.tree.setColumnWidth(0, 150)
        self.tree.header().setStretchLastSection(True)
        self.tree.setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)
        self.tree.setUniformRowHeights(True)
        self.tree.setAnimated(False)
        self.tree.setMinimumHeight(80)
        self.tree.itemSelectionChanged.connect(self._selected)
        tl.addWidget(self.tree, 1)

        find = QGroupBox("Find in files")
        fl = QVBoxLayout(find)
        fl.setContentsMargins(10, 10, 10, 10)
        fl.setSpacing(theme.ROW_GAP)
        row = QHBoxLayout()
        row.setSpacing(theme.ROW_GAP)
        self.find_box = QLineEdit()
        self.find_box.setPlaceholderText("text to find")
        self.find_box.returnPressed.connect(self.find)
        self.regex_check = QCheckBox("Regex")
        self.btn_find = QPushButton("Find")
        self.btn_find.clicked.connect(self.find)
        row.addWidget(self.find_box, 1)
        row.addWidget(self.regex_check)
        row.addWidget(self.btn_find)
        fl.addLayout(row)
        self.results = QListWidget()
        self.results.setObjectName("searchResults")
        self.results.setMinimumHeight(60)
        self.results.itemActivated.connect(self._open_result)
        self.results.itemClicked.connect(self._open_result)
        fl.addWidget(self.results, 1)

        # A splitter rather than fixed shares: a 300-hit search needs a tall results list while
        # browsing needs a tall tree, and which one you want changes as you work.
        split = QSplitter(Qt.Orientation.Vertical)
        split.setChildrenCollapsible(False)
        split.addWidget(top)
        split.addWidget(find)
        split.setStretchFactor(0, 3)
        split.setStretchFactor(1, 2)
        split.setSizes([240, 130])
        outer.addWidget(split, 1)
        return panel

    def _right_panel(self) -> QWidget:
        """The editor pane: an open-file tab strip, a breadcrumb, the code, and a status line."""
        panel = QWidget()
        outer = QVBoxLayout(panel)
        outer.setContentsMargins(0, 0, 0, 0)
        outer.setSpacing(0)

        self.tabs = _FileTabBar()
        self.tabs.setObjectName("fileTabs")
        self.tabs.setExpanding(False)
        self.tabs.setTabsClosable(False)     # the glyphs are painted by _FileTabBar
        self.tabs.setMovable(False)
        self.tabs.setUsesScrollButtons(True)
        self.tabs.currentChanged.connect(self._tab_selected)
        self.tabs.closeRequested.connect(self._close_tab)
        self.tabs.setVisible(False)
        outer.addWidget(self.tabs)

        crumb = QWidget()
        cl = QHBoxLayout(crumb)
        cl.setContentsMargins(2, 6, 2, 6)
        cl.setSpacing(theme.ROW_GAP)
        self.file_path = QLabel("")          # the breadcrumb: src › stgdglab › stgdglab_4.c
        self.file_path.setObjectName("breadcrumb")
        self.file_path.setTextInteractionFlags(Qt.TextInteractionFlag.TextSelectableByMouse)
        cl.addWidget(self.file_path)
        cl.addStretch(1)
        self.file_facts = QLabel("")
        self.file_facts.setObjectName("breadcrumb")
        cl.addWidget(self.file_facts)
        outer.addWidget(crumb)

        # Page 0 is the empty state, so the pane says what to do instead of looking broken. Every
        # file opened after that gets its own CodeView and its own tab.
        self.stack = QStackedWidget()
        empty = QWidget()
        el = QVBoxLayout(empty)
        el.setContentsMargins(0, 0, 0, 0)
        self.empty_label = QLabel("Select a file in the tree, or open a search result.")
        self.empty_label.setObjectName("emptyState")
        self.empty_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.empty_label.setWordWrap(True)
        el.addStretch(1)
        el.addWidget(self.empty_label)
        el.addStretch(1)
        self.stack.addWidget(empty)
        outer.addWidget(self.stack, 1)

        self.editor_status = QLabel("")
        self.editor_status.setObjectName("editorStatus")
        outer.addWidget(self.editor_status)
        return panel

    # -- the open files (an editor's tabs)
    @property
    def view(self) -> "CodeView | None":
        """The code view of the file on screen, or None while nothing is open."""
        current = self.stack.currentWidget()
        return current if isinstance(current, CodeView) else None

    def _status_text(self, rel: str) -> str:
        document = self._docs.get(rel)
        cursor = document.textCursor() if document else None
        line = cursor.blockNumber() + 1 if cursor else 1
        column = cursor.positionInBlock() + 1 if cursor else 1
        name = rel.rsplit("/", 1)[-1]
        kind = name.rsplit(".", 1)[-1].upper() if "." in name else "TEXT"
        return f"Ln {line}, Col {column}   ·   {kind}   ·   read-only"

    def _set_facts(self, rel: str) -> None:
        info = (next((f for f in self._inventory.files if f.rel == rel), None)
                if self._inventory else None)
        bits: list[str] = []
        if info:
            bits.append(human_size(info.size))
            bits.append(f"{info.lines:,} lines" if info.lines else "no line count")
            if info.funcs:
                bits.append(f"{info.funcs:,} functions counted")
            if info.stubs:
                bits.append(f"{info.stubs} stub{'s' if info.stubs != 1 else ''}")
            elif info.suffix == ".c":
                bits.append("no stubs")
        self.file_facts.setText("  ·  ".join(bits))
        self.file_path.setText("  ›  ".join(rel.split("/")))
        self.editor_status.setText(self._status_text(rel))

    def _tab_selected(self, index: int) -> None:
        if not (0 <= index < len(self._order)):
            return
        rel = self._order[index]
        self.stack.setCurrentWidget(self._docs[rel])
        # Moving the tree's selection fires itemSelectionChanged; the handler must not treat that
        # as the user opening a file, or it would re-open this one without the line.
        self._loading = True
        try:
            item = self._items.get(rel)
            if item is not None and item is not self.tree.currentItem():
                self.tree.setCurrentItem(item)
        finally:
            self._loading = False
        self._set_facts(rel)

    def _close_tab(self, index: int) -> None:
        if not (0 <= index < len(self._order)):
            return
        rel = self._order.pop(index)
        document = self._docs.pop(rel)
        self.stack.removeWidget(document)
        document.deleteLater()
        self.tabs.blockSignals(True)
        self.tabs.removeTab(index)
        self.tabs.blockSignals(False)
        if not self._order:
            self.tabs.setVisible(False)
            self.stack.setCurrentIndex(0)
            self.file_path.setText("")
            self.file_facts.setText("")
            self.editor_status.setText("")
            return
        self.tabs.setCurrentIndex(min(index, len(self._order) - 1))
        self._tab_selected(self.tabs.currentIndex())

    def _cursor_moved(self, rel: str) -> None:
        if self.view is self._docs.get(rel):
            self.editor_status.setText(self._status_text(rel))

    # -- data
    def ensure_loaded(self) -> None:
        """Scan once, the first time the tab is looked at, then keep showing the same tree."""
        if self._inventory is not None or not decomp.available():
            return
        self._inventory = decomp.scan()
        self._fill_tree()
        self.progress.set_status(decomp.published(), self._inventory.totals)

    def refresh(self) -> None:
        """Cheap: the published numbers and the identity line can change without the tree moving."""
        if not decomp.available():
            self.status.setText(f"Decomp tree not found at {decomp.ROOT}")
            self.tree.setEnabled(False)
            self.filter_box.setEnabled(False)
            self.find_box.setEnabled(False)
            self.btn_find.setEnabled(False)
            self.readonly.setText("")
            self.identity.setText("")
            self.progress.set_status({}, (0, 0, 0, 0))
            return
        self.progress.set_status(decomp.published(),
                                 self._inventory.totals if self._inventory else (0, 0, 0, 0))
        ident = decomp.identity()
        bits = [f"{r['region']} {r['binary']} · {r['sha1'][:12]}"
                for r in sorted(ident["regions"], key=lambda r: REGION_RANK.get(r["region"], 9))]
        self.identity.setText("   ".join(bits) if bits else decomp.ROOT.name)
        if self._inventory:
            files, lines, funcs, stubs = self._inventory.totals
            self.status.setText(f"{files:,} files · {lines:,} lines of code · {funcs:,} functions "
                                f"counted · {stubs} stubs · external/ not listed")

    def showEvent(self, event) -> None:  # noqa: N802 (Qt naming)
        super().showEvent(event)
        self.ensure_loaded()
        self.refresh()

    # -- tree
    def _fill_tree(self) -> None:
        assert self._inventory is not None
        self.tree.clear()
        self._items.clear()

        def add(node: decomp.Node, parent) -> None:
            item = QTreeWidgetItem([node.name, self._detail(node)])
            item.setData(0, Qt.ItemDataRole.UserRole, node.rel)
            item.setData(0, Qt.ItemDataRole.UserRole + 1, node.is_dir)
            font = item.font(0)
            font.setBold(node.is_dir)
            item.setFont(0, font)
            if parent is None:
                self.tree.addTopLevelItem(item)
            else:
                parent.addChild(item)
            self._items[node.rel] = item
            for child in node.children:
                add(child, item)

        for child in self._inventory.tree.children:
            add(child, None)

    @staticmethod
    def _detail(node: decomp.Node) -> str:
        if node.is_dir:
            # Only code files carry a line count, so a folder of config text says just its size in
            # files rather than advertising "0 lines".
            return (f"{node.files:,} files · {node.lines:,} lines" if node.lines
                    else f"{node.files:,} files")
        if node.lines:
            return f"{node.lines:,} lines · {node.funcs:,} functions"
        return human_size(node.size)

    def _apply_filter(self, text: str) -> None:
        needle = text.strip().lower()

        def visit(item: QTreeWidgetItem) -> bool:
            name = item.text(0).lower()
            self_hit = bool(needle) and needle in name
            child_hit = False
            for index in range(item.childCount()):
                child_hit = visit(item.child(index)) or child_hit
            item.setHidden(bool(needle) and not (self_hit or child_hit))
            return self_hit or child_hit

        for index in range(self.tree.topLevelItemCount()):
            visit(self.tree.topLevelItem(index))

    def _selected(self) -> None:
        if self._loading:          # our own open_path is driving the selection, not the user
            return
        items = self.tree.selectedItems()
        if not items:
            return
        item = items[0]
        if item.data(0, Qt.ItemDataRole.UserRole + 1):
            item.setExpanded(not item.isExpanded())
            return
        self.open_path(str(item.data(0, Qt.ItemDataRole.UserRole)))

    def open_path(self, rel: str, line: int | None = None) -> bool:
        """Show a file in its own tab, reading it the first time it is opened."""
        document = self._docs.get(rel)
        if document is None:
            document = CodeView()
            if not document.load(rel):
                document.deleteLater()
                self.file_path.setText("  ›  ".join(rel.split("/")))
                self.file_facts.setText("This file could not be read.")
                return False
            document.cursorPositionChanged.connect(lambda r=rel: self._cursor_moved(r))
            self._docs[rel] = document
            self._order.append(rel)
            self.stack.addWidget(document)
            self.tabs.blockSignals(True)
            self.tabs.addTab(rel.rsplit("/", 1)[-1])
            self.tabs.setTabToolTip(len(self._order) - 1, rel)
            self.tabs.setCurrentIndex(len(self._order) - 1)
            self.tabs.blockSignals(False)
            self.tabs.setVisible(True)
            while len(self._order) > MAX_TABS:
                self._close_tab(0)          # drop the oldest; the one just opened is last
        else:
            self.tabs.setCurrentIndex(self._order.index(rel))
        self.stack.setCurrentWidget(document)
        if line:
            document.goto_line(line)        # after it is on screen, so the scroll sticks
        self._set_facts(rel)
        item = self._items.get(rel)
        if item is not None and item is not self.tree.currentItem():
            self._loading = True
            try:
                self.tree.setCurrentItem(item)
            finally:
                self._loading = False
        return True

    # -- search
    def find(self) -> None:
        needle = self.find_box.text()
        self.results.clear()
        if not needle:
            return
        hits = decomp.search(needle, regex=self.regex_check.isChecked())
        if not hits:
            item = QListWidgetItem(f"no match for “{needle}”")
            item.setFlags(Qt.ItemFlag.NoItemFlags)
            self.results.addItem(item)
            return
        for rel, line, text in hits:
            entry = QListWidgetItem(f"{rel}:{line}    {text}")
            entry.setData(Qt.ItemDataRole.UserRole, (rel, line))
            self.results.addItem(entry)
        if len(hits) >= decomp.SEARCH_HITS:
            entry = QListWidgetItem(f"… first {decomp.SEARCH_HITS} matches shown")
            entry.setFlags(Qt.ItemFlag.NoItemFlags)
            self.results.addItem(entry)

    def _open_result(self, item: QListWidgetItem) -> None:
        data = item.data(Qt.ItemDataRole.UserRole)
        if isinstance(data, tuple):
            self.open_path(data[0], data[1])

    # -- theme
    def _restyle(self) -> None:
        for document in self._docs.values():
            document.restyle()
